#include <stdio.h>
#include <ctype.h>
#include <lcthw/dbg.h>
#include <lcthw/hashmap.h>
#include <lcthw/stats.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "net.h"
#include <netdb.h>

struct tagbstring LINE_SPLIT = bsStatic(" ");
struct tagbstring CREATE = bsStatic("create");
struct tagbstring STDDEV = bsStatic("stddev");
struct tagbstring MEAN = bsStatic("mean");
struct tagbstring SAMPLE = bsStatic("sample");
struct tagbstring DUMP = bsStatic("dump");
struct tagbstring DELETE = bsStatic("delete");
struct tagbstring OK = bsStatic("OK\n");
struct tagbstring ERR = bsStatic("ERR\n");
struct tagbstring DNE = bsStatic("DNE\n");
struct tagbstring EXISTS = bsStatic("EXISTS\n");
const char LINE_ENDING = '\n';

const int RB_SIZE = 1024 * 10;

Hashmap *DATA = NULL;

struct Command;

typedef int (*handler_cb)(struct Command *cmd, RingBuffer *send_rb);

typedef struct Command {
    bstring command;
    bstring name;
    bstring number;
    handler_cb handler;
} Command;


typedef struct Record {
    bstring name;
    Stats *stat;
} Record;

void handle_sigchild(int sig) {
    sig = 0; // ignore it
    while(waitpid(-1, NULL, WNOHANG) > 0) {
    }
}

void send_reply(RingBuffer *send_rb, bstring reply)
{
    RingBuffer_puts(send_rb, reply);
}

int handle_create(Command *cmd, RingBuffer *send_rb)
{
    int rc = 0;

    // if the name is in the DATA map then return exists
    if(Hashmap_get(DATA, cmd->name)) {
        send_reply(send_rb, &EXISTS);
    } else {
        // allocate a recrod
        debug("create: %s %s", bdata(cmd->name), bdata(cmd->number));

        Record *info = calloc(sizeof(Record), 1);
        check_mem(info);

        // set its stat element
        info->stat = Stats_create();
        check_mem(info->stat);

        // set its name element
        info->name = bstrcpy(cmd->name);
        check_mem(info->name);

        // do a first sample
        Stats_sample(info->stat, atof(bdata(cmd->number)));

        // add it to the hashmap
        rc = Hashmap_set(DATA, info->name, info);
        check(rc == 0, "Failed to add data to map.");

        // send an OK
        send_reply(send_rb, &OK);
    }

    return 0;
error:
    return -1;
}


int handle_sample(Command *cmd, RingBuffer *send_rb)
{
    // get the info from the hashmap
    Record *info = Hashmap_get(DATA, cmd->name);

    if(info == NULL) {
        // if it doesn't exist then DNE
        send_reply(send_rb, &DNE);
    } else {
        // else run sample on it, return the mean
        Stats_sample(info->stat, atof(bdata(cmd->number)));
        bstring reply = bformat("%f\n", Stats_mean(info->stat));
        send_reply(send_rb, reply);
        bdestroy(reply);
    }


    return 0;
}

int handle_delete(Command *cmd, RingBuffer *send_rb)
{
    log_info("delete: %s", bdata(cmd->name));
    Record *info = Hashmap_get(DATA, cmd->name);

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        Hashmap_delete(DATA, cmd->name);

        free(info->stat);
        bdestroy(info->name);
        free(info);

        send_reply(send_rb, &OK);
    }

    return 0;
}

int handle_mean(Command *cmd, RingBuffer *send_rb)
{
    log_info("mean: %s", bdata(cmd->name));
    Record *info = Hashmap_get(DATA, cmd->name);

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        bstring reply = bformat("%f\n", Stats_mean(info->stat));
        send_reply(send_rb, reply);
        bdestroy(reply);
    }

    return 0;
}

int handle_stddev(Command *cmd, RingBuffer *send_rb)
{
    log_info("stddev: %s", bdata(cmd->name));
    Record *info = Hashmap_get(DATA, cmd->name);

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        bstring reply = bformat("%f\n", Stats_stddev(info->stat));
        send_reply(send_rb, reply);
        bdestroy(reply);
    }

    return 0;
}

int handle_dump(Command *cmd, RingBuffer *send_rb)
{
    log_info("dump: %s", bdata(cmd->name));
    Record *info = Hashmap_get(DATA, cmd->name);

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        bstring reply = bformat("%f %f %f %f %ld %f %f\n",
                Stats_mean(info->stat),
                Stats_stddev(info->stat),
                info->stat->sum,
                info->stat->sumsq,
                info->stat->n,
                info->stat->min,
                info->stat->max);

        send_reply(send_rb, reply);
        bdestroy(reply);
    }

    return 0;
}

int parse_command(struct bstrList *splits, Command *cmd)
{
    // get the command
    cmd->command = splits->entry[0];

    if(biseq(cmd->command, &CREATE)) {
        check(splits->qty == 3, "Failed to parse create: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->number = splits->entry[2];
        cmd->handler = handle_create;
    } else if(biseq(cmd->command, &MEAN)) {
        check(splits->qty == 2, "Failed to parse mean: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_mean;
    } else if(biseq(cmd->command, &SAMPLE)) {
        check(splits->qty == 3, "Failed to parse sample: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->number = splits->entry[2];
        cmd->handler = handle_sample;
    } else if(biseq(cmd->command, &DUMP)) {
        check(splits->qty == 2, "Failed to parse dump: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_dump;
    } else if(biseq(cmd->command, &DELETE)) {
        check(splits->qty == 2, "Failed to parse delete: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_delete;
    } else if(biseq(cmd->command, &STDDEV)) {
        check(splits->qty == 2, "Failed to parse stddev: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_stddev;
    } else {
        sentinel("Failed to parse the command.");
    }

    return 0;
error:
    return -1;
}

int parse_line(bstring data, RingBuffer *send_rb)
{
    int rc = -1;
    Command cmd = {.command = NULL};

    // split data on line boundaries
    struct bstrList *splits = bsplits(data, &LINE_SPLIT);
    check(splits != NULL, "Bad data.");

    // parse it into a command
    rc = parse_command(splits, &cmd);
    check(rc == 0, "Failed to parse command.");

    // call the command handler for that command
    rc = cmd.handler(&cmd, send_rb);

error: // fallthrough
    if(splits) bstrListDestroy(splits);
    return rc;

}

void client_handler(int client_fd)
{
    int rc = 0;
    RingBuffer *recv_rb = RingBuffer_create(RB_SIZE);
    RingBuffer *send_rb = RingBuffer_create(RB_SIZE);

    check_mem(recv_rb);
    check_mem(send_rb);

    // keep reading into the recv buffer and sending on send
    while(read_some(recv_rb, client_fd, 1) != -1) {
        // read a line from the recv_rb
        bstring data = read_line(recv_rb, LINE_ENDING);
        check(data != NULL, "Client closed.");

        // parse it, close on any protocol errors
        rc = parse_line(data, send_rb);
        bdestroy(data); // cleanup here
        check(rc == 0, "Failed to parse user. Closing.");

        // and as long as there's something to send, send it
        if(RingBuffer_available_data(send_rb)) {
            write_some(send_rb, client_fd, 1);
        }
    }

    // close the socket
    rc = close(client_fd);
    check(rc != -1, "Failed to close the socket.");

error: // fallthrough
    if(recv_rb) RingBuffer_destroy(recv_rb);
    if(send_rb) RingBuffer_destroy(send_rb);
    exit(0); // just exit the child process
}

int setup_data_store()
{
    // a more advanced design simply wouldn't use this
    DATA = Hashmap_create(NULL, NULL);
    check_mem(DATA);

    return 0;
error:
    return -1;
}

int echo_server(const char *host, const char *port)
{
    int rc = 0;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(client_addr);
    int server_socket = 0;
    int client_fd = 0;

    rc = setup_data_store();
    check(rc == 0, "Failed to setup the data store.");

    struct sigaction sa = {
        .sa_handler = handle_sigchild,
        .sa_flags = SA_RESTART | SA_NOCLDSTOP
    };

    check(host != NULL, "Invalid host.");
    check(port != NULL, "Invalid port.");

    // create a sigaction that handles SIGCHLD
    sigemptyset(&sa.sa_mask);
    rc = sigaction(SIGCHLD, &sa, 0);
    check(rc != -1, "Failed to setup signal handler for child processes.");

    // listen on the given port and host
    server_socket = server_listen(host, port);
    check(server_socket >= 0, "bind to %s:%s failed.", host, port);

    while(1) {
        // accept the connection
        client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size); 
        check(client_fd >= 0, "Failed to accept connection.");

        debug("Client connected.");

        rc = fork();
        if(rc == 0) {
            // child process
            close(server_socket); // don't need this
            // handle the client
            client_handler(client_fd);
        } else {
            // server process
            close(client_fd); // don't need this
        }
    }

error:  // fallthrough
    return -1;
}
