#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include <lcthw/ringbuffer.h>
#include <lcthw/dbg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include "net.h"

struct tagbstring NL = bsStatic("\n");
struct tagbstring CRLF = bsStatic("\r\n");

int nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    check(flags >= 0, "Invalid flags on nonblock.");

    int rc = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    check(rc == 0, "Can't set nonblocking.");

    return 0;
error:
    return -1;
}

int client_connect(char *host, char *port)
{
    int rc = 0;
    struct addrinfo *addr = NULL;

    rc = getaddrinfo(host, port, NULL, &addr);
    check(rc == 0, "Failed to lookup %s:%s", host, port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    check(sock >= 0, "Cannot create a socket.");

    rc = connect(sock, addr->ai_addr, addr->ai_addrlen);
    check(rc == 0, "Connect failed.");

    rc = nonblock(sock);
    check(rc == 0, "Can't set nonblocking.");

    freeaddrinfo(addr);
    return sock;

error:
    freeaddrinfo(addr);
    return -1;
}

int read_some(RingBuffer * buffer, int fd, int is_socket)
{
    int rc = 0;

    if (RingBuffer_available_data(buffer) == 0) {
        buffer->start = buffer->end = 0;
    }

    if (is_socket) {
        rc = recv(fd, RingBuffer_starts_at(buffer),
                RingBuffer_available_space(buffer), 0);
    } else {
        rc = read(fd, RingBuffer_starts_at(buffer),
                RingBuffer_available_space(buffer));
    }

    check(rc >= 0, "Failed to read from fd: %d", fd);

    RingBuffer_commit_write(buffer, rc);

    return rc;

error:
    return -1;
}

int write_some(RingBuffer * buffer, int fd, int is_socket)
{
    int rc = 0;
    bstring data = RingBuffer_get_all(buffer);

    check(data != NULL, "Failed to get from the buffer.");
    check(bfindreplace(data, &NL, &CRLF, 0) == BSTR_OK,
            "Failed to replace NL.");

    if (is_socket) {
        rc = send(fd, bdata(data), blength(data), 0);
    } else {
        rc = write(fd, bdata(data), blength(data));
    }

    check(rc == blength(data), "Failed to write everything to fd: %d.",
            fd);
    bdestroy(data);

    return rc;

error:
    return -1;
}

int attempt_listen(struct addrinfo *info)
{
    int sockfd = -1; // default fail
    int rc = -1;
    int yes = 1;

    check(info != NULL, "Invalid addrinfo.");

    // create a socket with the addrinfo
    sockfd = socket(info->ai_family, info->ai_socktype,
            info->ai_protocol);
    check_debug(sockfd != -1, "Failed to bind to address. Trying more.");

    // set the SO_REUSEADDR option on the socket
    rc = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    check_debug(rc == 0, "Failed to set SO_REUSADDR.");

    // attempt to bind to it
    rc = bind(sockfd, info->ai_addr, info->ai_addrlen);
    check_debug(rc == 0, "Failed to find socket.");

    // finally listen with a backlog
    rc = listen(sockfd, BACKLOG);
    check_debug(rc == 0, "Failed to listen to socket.");

    return sockfd;

error:
    return -1;
}


int server_listen(const char *host, const char *port)
{
    int rc = 0;
    int sockfd = -1; // default fail value
    struct addrinfo *info = NULL;
    struct addrinfo *next_p = NULL;
    struct addrinfo addr = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE
    };

    check(host != NULL, "Invalid host.");
    check(port != NULL, "Invalid port.");

    // get the address info for host and port
    rc = getaddrinfo(NULL, port, &addr, &info);
    check(rc == 0, "Failed to get address info for connect.");

    // cycle through the available list to find one
    for(next_p = info; next_p != NULL; next_p = next_p->ai_next)
    {
        // attempt to listen to each one
        sockfd = attempt_listen(next_p);
        if(sockfd != -1) break;
    }

    // either we found one and were able to listen or nothing.
    check(sockfd != -1, "All possible addresses failed.");

error: //fallthrough
    if(info) freeaddrinfo(info);
    // this gets set by the above to either -1 or valid
    return sockfd;
}

bstring read_line(RingBuffer *input, const char line_ending)
{
    int i = 0;
    bstring result = NULL;

    // not super efficient
    // read a character at a time from the ring buffer
    for(i = 0; i < RingBuffer_available_data(input); i++) {
        // if the buffer has line ending
        if(input->buffer[i] == line_ending) {
            // get that much fromt he ring buffer
            result = RingBuffer_gets(input, i);
            check(result, "Failed to get line from RingBuffer");
            // make sure that we got the right amount
            check(RingBuffer_available_data(input) >= 1, 
                    "Not enough data in the RingBuffer after reading line.");
            // and commit it
            RingBuffer_commit_read(input, 1);
            break;
        }
    }

    // notice this will fail in the cases where we get a set of data
    // on the wire that does not have a line ending yet
    return result;
error:
    return NULL;
}


void send_reply(RingBuffer *send_rb, bstring reply)
{
    RingBuffer_puts(send_rb, reply);
}

