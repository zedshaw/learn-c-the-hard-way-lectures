#include <stdio.h>
#include <ctype.h>
#include <lcthw/dbg.h>
#include <lcthw/hashmap.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "net.h"
#include <netdb.h>
#include "statserve.h"

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
struct tagbstring SLASH = bsStatic("/");
const char LINE_ENDING = '\n';

const int RB_SIZE = 1024 * 10;

Hashmap *DATA = NULL;

void handle_sigchild(int sig) {
    sig = 0; // ignore it
    while(waitpid(-1, NULL, WNOHANG) > 0) {
    }
}


int handle_create(Command *cmd, RingBuffer *send_rb, bstring path)
{
    int rc = 0;
    int is_root = biseq(path, cmd->name);
    log_info("create: %s %s %s", bdata(cmd->name), bdata(path), bdata(cmd->number));

    Record *info = Hashmap_get(DATA, path);

    if(info != NULL && is_root) {
        // report if root exists, just skip children
        send_reply(send_rb, &EXISTS);
    } else if(info != NULL) {
        debug("Child %s exists, skipping it.", bdata(path));
        return 0;
    } else {
        // new child so make it
        debug("create: %s %s", bdata(path), bdata(cmd->number));

        Record *info = calloc(sizeof(Record), 1);
        check_mem(info);

        // set its stat element
        info->stat = Stats_create();
        check_mem(info->stat);

        // set its name element
        info->name = bstrcpy(path);
        check_mem(info->name);

        // do a first sample
        Stats_sample(info->stat, atof(bdata(cmd->number)));

        // add it to the hashmap
        rc = Hashmap_set(DATA, info->name, info);
        check(rc == 0, "Failed to add data to map.");

        // only send the for the root part
        if(is_root) {
            send_reply(send_rb, &OK);
        }
    }

    return 0;
error:
    return -1;
}


int handle_sample(Command *cmd, RingBuffer *send_rb, bstring path)
{
    // get the info from the hashmap
    Record *info = Hashmap_get(DATA, path);
    int is_root = biseq(path, cmd->name);
    log_info("sample %s %s %s", bdata(cmd->name), bdata(path), bdata(cmd->number));
    bstring child_path = NULL;

    if(info == NULL) {
        // if it doesn't exist then DNE
        send_reply(send_rb, &DNE);
        return 0;
    } else {
        if(is_root) {
            // just sample the root like normal
            Stats_sample(info->stat, atof(bdata(cmd->number)));
        } else {
            // need to do some hackery to get the child path
            // for rolling up mean-of-means on it

            // increase the qty on path up one
            cmd->path->qty++;
            // get the "child path" (previous path?)
            child_path = bjoin(cmd->path, &SLASH);
            // get that info from the DATA
            Record *child_info = Hashmap_get(DATA, child_path);
            bdestroy(child_path);

            // if it exists then sample on it
            if(child_info) {
                // info is /logins, child_info is /logins/zed 
                // we want /logins/zed's mean to be a new sample on /logins
                Stats_sample(info->stat, Stats_mean(child_info->stat));
            }
            // drop the path back to where it was
            cmd->path->qty--;
        }

    }

    // do the reply for the mean last
    bstring reply = bformat("%f\n", Stats_mean(info->stat));
    send_reply(send_rb, reply);
    bdestroy(reply);

    return 0;
}

int handle_delete(Command *cmd, RingBuffer *send_rb, bstring path)
{
    log_info("delete: %s %s", bdata(cmd->name), bdata(path));
    Record *info = Hashmap_get(DATA, path);
    int is_root = biseq(path, cmd->name);

    // BUG: should just decide that this isn't scanned 
    // but run once, for now just only run on root
    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else if(is_root) {
        Hashmap_delete(DATA, path);

        free(info->stat);
        bdestroy(info->name);
        free(info);

        send_reply(send_rb, &OK);
    }

    return 0;
}

int handle_mean(Command *cmd, RingBuffer *send_rb, bstring path)
{
    log_info("mean: %s %s %s", bdata(cmd->name), bdata(path), bdata(path));
    Record *info = Hashmap_get(DATA, path);

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        bstring reply = bformat("%f\n", Stats_mean(info->stat));
        send_reply(send_rb, reply);
        bdestroy(reply);
    }

    return 0;
}

int handle_stddev(Command *cmd, RingBuffer *send_rb, bstring path)
{
    log_info("stddev: %s %s %s", bdata(cmd->name), bdata(path), bdata(path));
    Record *info = Hashmap_get(DATA, path);

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        bstring reply = bformat("%f\n", Stats_stddev(info->stat));
        send_reply(send_rb, reply);
        bdestroy(reply);
    }

    return 0;
}

int handle_dump(Command *cmd, RingBuffer *send_rb, bstring path)
{
    log_info("dump: %s, %s, %s", bdata(cmd->name), bdata(path), bdata(path));
    Record *info = Hashmap_get(DATA, path);

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

int scan_paths(Command *cmd, RingBuffer *send_rb)
{
    check(cmd->path != NULL, "Path was not set in command.");

    int rc = 0;
    // save the original path length
    size_t qty = cmd->path->qty;

    // starting at the longest path, shorten it and call
    // for each one:
    for(; cmd->path->qty > 1; cmd->path->qty--) {
        // remake the path with / again
        bstring path = bjoin(cmd->path, &SLASH);
        // call the handler with the path
        rc = cmd->handler(cmd, send_rb, path);
        // if the handler returns != 0 then abort and return that
        bdestroy(path);

        if(rc != 0) break;
    }

    // restore path length
    cmd->path->qty = qty;
    return rc;
error:
    return -1;
}

struct bstrList *parse_name(bstring name)
{
    return bsplits(name, &SLASH);
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

    // parse the name into the path we need for scan_paths
    cmd.path = parse_name(cmd.name);
    check(cmd.path != NULL, "Invalid path.");

    // scan the path and call the handlers
    rc = scan_paths(&cmd, send_rb);
    check(rc == 0, "Failure running command against path: %s", bdata(cmd.name));

    bstrListDestroy(cmd.path);
    bstrListDestroy(splits);

    return 0;

error: // fallthrough
    if(cmd.path) bstrListDestroy(cmd.path);
    if(splits) bstrListDestroy(splits);
    return -1;
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
