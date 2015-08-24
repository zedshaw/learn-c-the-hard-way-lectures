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
#include <fcntl.h>
#include "statserve.h"

struct tagbstring LINE_SPLIT = bsStatic(" ");
struct tagbstring CREATE = bsStatic("create");
struct tagbstring STDDEV = bsStatic("stddev");
struct tagbstring MEAN = bsStatic("mean");
struct tagbstring SAMPLE = bsStatic("sample");
struct tagbstring DUMP = bsStatic("dump");
struct tagbstring DELETE = bsStatic("delete");
struct tagbstring STORE = bsStatic("store");
struct tagbstring LOAD = bsStatic("load");
struct tagbstring OK = bsStatic("OK\n");
struct tagbstring ERR = bsStatic("ERR\n");
struct tagbstring DNE = bsStatic("DNE\n");
struct tagbstring EXISTS = bsStatic("EXISTS\n");
struct tagbstring SLASH = bsStatic("/");
const char LINE_ENDING = '\n';

const int RB_SIZE = 1024 * 10;

Hashmap *DATA = NULL;
bstring STORE_PATH = NULL;

void handle_sigchild(int sig) {
    sig = 0; // ignore it
    while(waitpid(-1, NULL, WNOHANG) > 0) {
    }
}

// BUG: this is stupid, use md5
void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

/// TOTALLY RANDOM! LOL  BUG: not secure
const uint32_t STORE_KEY[4] = {18748274, 228374, 193034845, 85726348};
struct tagbstring FAUX64 = bsStatic("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");

// BUG: this all dies
bstring encrypt_armor_name(bstring name)
{
    // copy the name to encrypt
    bstring encname = bstrcpy(name);
    size_t i = 0;
    // point the encrypt pointer at it
    // BUG: this cast is weird, why?
    uint32_t *v = (uint32_t *)bdata(encname);

    // extend the encname so that it can hold everything
    // BUG: use a correct padding algorithm
    while(blength(encname) % (sizeof(uint32_t) * 2) > 0) {
        bconchar(encname, ' ');
    }

    // run encipher on this
    // BUG: get rid of encipher
    for(i = 0; i < (size_t)blength(encname) / (sizeof(uint32_t) * 2); i+=2) {
        encipher(1, v+i, STORE_KEY);
    }

    // do a lame "base 64" kind of thing on it
    // BUG: this is NOT the best way, it's a quick hack to get it working
    // replace with real BASE64 later
    for(i = 0; i < (size_t)blength(encname); i++) {
        int at = encname->data[i] % blength(&FAUX64);
        encname->data[i] = FAUX64.data[at];
    }

    // that's our final hack encrypted name
    return encname;
}


bstring sanitize_location(bstring base, bstring path)
{
    bstring attempt = NULL;
    bstring encpath = NULL;
    
    // encrypt armore the name
    // BUG: ditch encryption, it was dumb
    encpath = encrypt_armor_name(path);
    check(encpath != NULL, "Failed to encrypt path name: %s", bdata(path));

    // combine it with the base, this means that we've armored the 
    // path so we can just append it
    attempt = bformat("%s/%s", bdata(base), bdata(encpath));
    bdestroy(encpath);
    return attempt;

error:
    if(encpath) bdestroy(encpath);
    if(attempt) bdestroy(attempt);
    return NULL;
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

        Record *info = calloc(1, sizeof(Record));
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
    log_info("delete: %s", bdata(cmd->name));
    Record *info = Hashmap_get(DATA, cmd->name);
    check(path == NULL && cmd->path == NULL, "Should be a recursive command.");

    // BUG: should just decide that this isn't scanned 
    // but run once, for now just only run on root
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
error:
    return -1;
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


int handle_store(Command *cmd, RingBuffer *send_rb, bstring path)
{
    Record *info = Hashmap_get(DATA, cmd->name);
    bstring location = NULL;
    bstring from = cmd->name;
    int rc = 0;
    int fd = -1;

    check(cmd != NULL, "Invalid command.");
    debug("store %s", bdata(cmd->name));
    check(path == NULL && cmd->path == NULL, "Store is non-recursive.");

    if(info == NULL) {
        send_reply(send_rb, &DNE);
    } else {
        // it exists so we sanitize the name
        location = sanitize_location(STORE_PATH, from);
        check(location, "Failed to sanitize the location.");

        // open the file we need with EXLOCK
        fd = open(bdata(location), O_WRONLY | O_CREAT | O_EXLOCK, S_IRWXU);
        check(fd >= 0, "Cannot open file for writing: %s", bdata(location));

        // write the Stats part of info to it
        rc = write(fd, info->stat, sizeof(Stats));
        check(rc == sizeof(Stats), "Failed to write to %s", bdata(location));

        // close, which should release the lock
        close(fd);

        // then send OK
        send_reply(send_rb, &OK);
    }

    return 0;
error: 
    if(fd < 0) close(fd);
    return -1;
}

int handle_load(Command *cmd, RingBuffer *send_rb, bstring path)
{
    bstring to = cmd->arg;
    bstring from = cmd->name;
    bstring location = NULL;
    Record *info = Hashmap_get(DATA, to);
    int fd = -1;

    check(path == NULL && cmd->path == NULL, "Load is non-recursive.");

    if(info != NULL) {
        // don't do it if the target to exists
        send_reply(send_rb, &EXISTS);
    } else {
        location = sanitize_location(STORE_PATH, from);
        check(location, "Failed to sanitize location.");

        // make a new record for the to target
        // TODO: make regular CRUD methods for Record
        info = calloc(1, sizeof(Record));
        check_mem(info);

        info->stat = calloc(1, sizeof(Stats));
        check_mem(info->stat);

        // open the file to read from readonly and locked
        fd = open(bdata(location), O_RDONLY | O_EXLOCK);
        check(fd >= 0, "Error opening file: %s", bdata(location));

        // read into the stats record 
        int rc = read(fd, info->stat, sizeof(Stats));
        check(rc == sizeof(Stats), "Failed to read record at %s", bdata(location));

        // close so we release the lock quick
        close(fd);

        // make a copy of to as the name for the info
        info->name = bstrcpy(to);
        check_mem(info->name);

        // put it in the hashmap
        rc = Hashmap_set(DATA, info->name, info);
        check(rc == 0, "Failed to ass to data map: %s", bdata(info->name));

        // and send the reply
        send_reply(send_rb, &OK);
    }

    return 0;
error:
    if(fd < 0) close(fd);
    return -1;
}

int parse_command(struct bstrList *splits, Command *cmd)
{
    check(splits != NULL, "Invalid split line.");

    // get the command
    cmd->command = splits->entry[0];

    if(biseq(cmd->command, &CREATE)) {
        check(splits->qty == 3, "Failed to parse create: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->number = splits->entry[2];
        cmd->handler = handle_create;
        cmd->path = parse_name(cmd->name);
    } else if(biseq(cmd->command, &MEAN)) {
        check(splits->qty == 2, "Failed to parse mean: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_mean;
        cmd->path = parse_name(cmd->name);
    } else if(biseq(cmd->command, &SAMPLE)) {
        check(splits->qty == 3, "Failed to parse sample: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->number = splits->entry[2];
        cmd->handler = handle_sample;
        cmd->path = parse_name(cmd->name);
    } else if(biseq(cmd->command, &DUMP)) {
        check(splits->qty == 2, "Failed to parse dump: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_dump;
        cmd->path = parse_name(cmd->name);
    } else if(biseq(cmd->command, &DELETE)) {
        check(splits->qty == 2, "Failed to parse delete: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_delete;
        cmd->path = NULL;
    } else if(biseq(cmd->command, &STDDEV)) {
        check(splits->qty == 2, "Failed to parse stddev: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_stddev;
        cmd->path = parse_name(cmd->name);
    } else if(biseq(cmd->command, &STORE)) {
        // store URL
        check(splits->qty == 2, "Failed to parse store: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->handler = handle_store;
        cmd->path = NULL;
    } else if(biseq(cmd->command, &LOAD)) {
        // load FROM TO
        check(splits->qty == 3, "Failed to parse load: %d", splits->qty);
        cmd->name = splits->entry[1];
        cmd->arg = splits->entry[2];
        cmd->handler = handle_load;
        cmd->path = NULL;
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

    // scan the path and call the handlers
    if(cmd.path) { 
        check(cmd.path->qty > 1, "Didn't give a valid URL.");
        rc = scan_paths(&cmd, send_rb);
        check(rc == 0, "Failure running recursive command against path: %s", bdata(cmd.name));
        bstrListDestroy(cmd.path);
    } else {
        rc = cmd.handler(&cmd, send_rb, NULL);
        check(rc == 0, "Failed running command against path: %s", bdata(cmd.name));
    }

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

int setup_data_store(const char *store_path)
{
    // a more advanced design simply wouldn't use this
    DATA = Hashmap_create(NULL, NULL);
    check_mem(DATA);

    char *path = realpath(store_path, NULL);
    check(path != NULL, "Failed to get the real path for storage: %s", store_path);
    
    STORE_PATH = bfromcstr(path);
    free(path);

    return 0;
error:
    return -1;
}

int run_server(const char *host, const char *port, const char *store_path)
{
    int rc = 0;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(client_addr);
    int server_socket = 0;
    int client_fd = 0;

    rc = setup_data_store(store_path);
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
