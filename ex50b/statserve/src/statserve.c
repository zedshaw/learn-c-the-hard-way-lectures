#include <lcthw/ringbuffer.h>
#include <lcthw/dbg.h>
#include <lcthw/hashmap.h>
#include <lcthw/stats.h>
#include <unistd.h>
#include <stdlib.h>
#include "statserve.h"
#include "net.h"
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/file.h>

const int RB_SIZE = 1024 * 10;
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

// this is just temporary to work out the protocol
// it actually doesn't work in practice because forking
Hashmap *DATA = NULL;

struct Command;

typedef int (*handler_cb)(struct Command *cmd, RingBuffer *send_rb, bstring path);

typedef struct Command {
    bstring command;
    bstring name;
    struct bstrList *path;
    bstring number;
    handler_cb handler;
} Command;

typedef struct Record {
    bstring name;
    Stats *stat;
} Record;

int handle_create(Command *cmd, RingBuffer *send_rb, bstring path)
{
    Record *info = Hashmap_get(DATA, path);

    // BUG: does duplicates of children
    if(info != NULL && biseq(path, cmd->name)) {
        // report the root exists, don't report children
        send_reply(send_rb, &EXISTS);
    } else if(info != NULL) {
        // skip children and don't overwrite them
        debug("Child exists so skipping it.");
        return 0;
    } else {
        // brand spanking new so make it
        debug("create: %s %s", bdata(path), bdata(cmd->number));
        Record *info = calloc(sizeof(Record), 1);
        check_mem(info);

        info->stat = Stats_create();
        check_mem(info->stat);

        info->name = bstrcpy(path);
        check_mem(info->name);

        Stats_sample(info->stat, atof(bdata(cmd->number)));
        
        Hashmap_set(DATA, info->name, info);

        // only send the OK on the last path part
        if(cmd->path->qty == 2) {
            send_reply(send_rb, &OK);
        }
    }

    return 0;
error:
    return -1;
}

int handle_mean(Command *cmd, RingBuffer *send_rb, bstring path)
{
    debug("mean: %s from %s", bdata(path), bdata(cmd->name));
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

int handle_sample(Command *cmd, RingBuffer *send_rb, bstring path)
{
    debug("sample: %s %s", bdata(path), bdata(cmd->number));
    Record *info = Hashmap_get(DATA, path);
    bstring child_path = NULL;

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        debug("found info: '%s' vs. '%s'", bdata(path), bdata(cmd->name));

        if(biseq(path, cmd->name)) {
            Stats_sample(info->stat, atof(bdata(cmd->number)));
        } else {
            // need to do a bit of hackery to get the child path
            // then we can do a mean-of-means on it
            cmd->path->qty++;
            child_path = bjoin(cmd->path, &SLASH);
            debug("child_path: %s", bdata(child_path));
            Record *child_info = Hashmap_get(DATA, child_path);

            if(child_info) {
                Stats_sample(info->stat, Stats_mean(child_info->stat));
            }

            cmd->path->qty--; // drop it back down to continue 
        }
    }

    bstring reply = bformat("%f\n", Stats_mean(info->stat));
    send_reply(send_rb, reply);
    bdestroy(reply);

    return 0;
}

int handle_dump(Command *cmd, RingBuffer *send_rb, bstring path)
{
    debug("dump:  %s from %s", bdata(path), bdata(cmd->name));
    Record *info = Hashmap_get(DATA, path);

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        bstring reply = bformat("%s %f %f %f %f %ld %f %f\n",
                bdata(info->name),
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

int handle_delete(Command *cmd, RingBuffer *send_rb, bstring path)
{
    debug("delete: %s from %s", bdata(path), bdata(cmd->name));
    Record *info = Hashmap_get(DATA, path);

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        free(info->stat);
        bdestroy(info->name);
        free(info);
        Hashmap_delete(DATA, path);
        send_reply(send_rb, &OK);
    }

    return 0;
}

int handle_stddev(Command *cmd, RingBuffer *send_rb, bstring path)
{
    debug("stddev: %s from %s", bdata(path), bdata(cmd->name)); 
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

int scan_paths(Command *cmd, RingBuffer *send_rb)
{
    size_t qty = cmd->path->qty;
    int rc = 0;

    for(;cmd->path->qty > 1; cmd->path->qty--) {
        bstring path = bjoin(cmd->path, &SLASH);
        rc = cmd->handler(cmd, send_rb, path);
        bdestroy(path);
        if(rc != 0) break;
    }

    cmd->path->qty = qty;
    return rc;
}

struct bstrList *parse_name(bstring name)
{
    return bsplits(name, &SLASH);
}

int parse_command(struct bstrList *splits, Command *cmd)
{
    cmd->command = splits->entry[0];

    if(biseq(cmd->command, &CREATE)) {
        check(splits->qty == 3, "Failed to parse create: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->path = parse_name(cmd->name);
        cmd->number = splits->entry[2];
        cmd->handler = handle_create;
    } else if(biseq(cmd->command, &MEAN)) {
        check(splits->qty == 2, "Failed to parse mean: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->path = parse_name(cmd->name);
        cmd->handler = handle_mean;
    } else if(biseq(cmd->command, &SAMPLE)) {
        check(splits->qty == 3, "Failed to parse sample: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->path = parse_name(cmd->name);
        cmd->number = splits->entry[2];
        cmd->handler = handle_sample;
    } else if(biseq(cmd->command, &DUMP)) {
        check(splits->qty == 2, "Failed to parse dump: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->path = parse_name(cmd->name);
        cmd->handler = handle_dump;
    } else if(biseq(cmd->command, &DELETE)) {
        check(splits->qty == 2, "Failed to parse delete: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->path = parse_name(cmd->name);
        cmd->handler = handle_delete;
    } else if(biseq(cmd->command, &STDDEV)) {
        check(splits->qty == 2, "Failed to parse stddev: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->path = parse_name(cmd->name);
        cmd->handler = handle_stddev;
    } else {
        sentinel("Failed to parse the command.");
    }


    return 0;
error:
    return 1;
}

int parse_line(bstring data, RingBuffer *send_rb)
{
    int rc = 0;
    Command cmd = {.command = NULL};

    struct bstrList *splits = bsplits(data, &LINE_SPLIT);
    check(splits != NULL, "Bad data.");

    rc = parse_command(splits, &cmd);
    check(rc == 0, "Failed to parse command.");
    check(cmd.path->qty > 1, "Didn't give a valid URL.");

    // they used a path so break it up
    rc = scan_paths(&cmd, send_rb);
    check(rc == 0, "Failure running command against path: %s", bdata(cmd.name));

    bstrListDestroy(cmd.path);
    bstrListDestroy(splits);

    return rc;
error:

    if(cmd.path) bstrListDestroy(cmd.path);
    if(splits) bstrListDestroy(splits);
    return -1;
}


int client_handler(int fd)
{
    int rc = 0;
    RingBuffer *recv_rb = RingBuffer_create(RB_SIZE);
    RingBuffer *send_rb = RingBuffer_create(RB_SIZE);

    check_mem(recv_rb);
    check_mem(send_rb);

    while(read_some(recv_rb, fd, 1) != -1) {
        // TODO: read a line, put the rest back
        bstring data = read_line(recv_rb, LINE_ENDING);
        check(data != NULL, "Client closed.");

        rc = parse_line(data, send_rb);
        check(rc == 0, "Failed to parse user. Closing.");

        if(RingBuffer_available_data(send_rb)) {
            write_some(send_rb, fd, 1);
        }

        bdestroy(data);
    }

    rc = close(fd);
    check(rc != -1, "Failed to close fd.");

error: // fallthrough
    if(recv_rb) RingBuffer_destroy(recv_rb);
    exit(0);
}

void handle_sigchld(int sig) {
    if(sig == SIGCHLD) {
        while(waitpid(-1, NULL, WNOHANG) > 0) {
        }
    }
}

int run_server(const char *host, const char *port)
{
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(client_addr);
    int server_socket = 0;
    int client_fd = 0;
    int rc = 0;

    DATA = Hashmap_create(NULL, NULL);
    check_mem(DATA);

    struct sigaction sa = {
        .sa_handler = handle_sigchld, 
        .sa_flags = SA_RESTART | SA_NOCLDSTOP
    };
    sigemptyset(&sa.sa_mask);

    rc = sigaction(SIGCHLD, &sa, 0);
    check(rc != -1, "Failed to setup signal handler for child processes.");

    server_socket = server_listen(host, port);
    check(server_socket >= 0, "bind to %s:%s failed.",
            host, port);

    while (1) {
        client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);
        check(client_fd >= 0, "Failed to accept connection.");

        debug("Client connected.");

        rc = fork();
        check(rc != -1, "Failed to fork!");

        if(rc == 0) {
            // in the child process
            close(server_socket);
            client_handler(client_fd);
        } else {
            // server process doesn't need this
            close(client_fd);
        }
    }

    return 0;

error:
    return -1;
}

