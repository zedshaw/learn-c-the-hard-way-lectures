#ifndef _net_h
#define _net_h

#include <lcthw/ringbuffer.h>

#define BACKLOG 10

int nonblock(int fd);
int client_connect(char *host, char *port);
int read_some(RingBuffer * buffer, int fd, int is_socket);
int write_some(RingBuffer * buffer, int fd, int is_socket);
int server_listen(const char *host, const char *port);
bstring read_line(RingBuffer *input, const char line_ending);
void send_reply(RingBuffer *send_rb, bstring reply);


#endif
