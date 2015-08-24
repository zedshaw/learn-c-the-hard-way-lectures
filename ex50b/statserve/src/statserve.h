#ifndef _statserve_h
#define _statserve_h

#include <lcthw/bstrlib.h>
#include <lcthw/ringbuffer.h>
#include <lcthw/stats.h>

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

struct tagbstring OK;

int setup_data_store();

struct bstrList *parse_name(bstring name);

int scan_paths(Command *cmd, RingBuffer *send_rb);

int parse_line(bstring data, RingBuffer *send_rb);

int echo_server(const char *host, const char *port);

#endif
