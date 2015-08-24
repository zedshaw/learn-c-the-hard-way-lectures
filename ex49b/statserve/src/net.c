#include <lcthw/ringbuffer.h>
#include <lcthw/bstrlib.h>
#include <lcthw/dbg.h>
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
    int sockfd = 0;
    int rc = -1;
    int yes = 1;

    sockfd = socket(info->ai_family, info->ai_socktype,
            info->ai_protocol);
    check_debug(sockfd != -1, "Failed to bind to address result. Trying more.");

    rc = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    check_debug(rc == 0, "Failed to set SO_REISEADDR.");

    rc = bind(sockfd, info->ai_addr, info->ai_addrlen);
    check_debug(rc == 0, "Failed to bind socket.");
   
    rc = listen(sockfd, BACKLOG);
    check_debug(rc == 0, "Failed to listen to socket.");

    return sockfd;

error:
    return -1;
}

int server_listen(const char *host, const char *port)
{
    int rc = 0;
    int sockfd = -1;
    struct addrinfo *info = NULL;
    struct addrinfo *next_p = NULL;
    struct addrinfo addr = {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE
    };

    check(host != NULL, "Must give a valid host.");
    check(port != NULL, "Must have a valid port.");

    rc = getaddrinfo(NULL, port, &addr, &info);
    check(rc == 0, "Failed to get address info for connect.");
 
    for(next_p = info; next_p != NULL; next_p = next_p->ai_next)
    {
        sockfd = attempt_listen(next_p);
        if(sockfd != -1) break;
    }

    check(sockfd != -1, "All possible addresses failed.");

    if(info) freeaddrinfo(info);
    return sockfd;
    
error: // fallthrough
    if(info) freeaddrinfo(info);
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


bstring read_line(RingBuffer *input, const char line_ending)
{
    int i = 0;
    bstring result = NULL;

    for(i = 0; i < RingBuffer_available_data(input); i++) {
        if(input->buffer[i] == line_ending) {
            result = RingBuffer_gets(input, i);
            check(result, "Failed to get line from RingBuffer.");
            check(RingBuffer_available_data(input) >= 1, "Not enough data in the RingBuffer after reading a line.");
            // eat the \n in the buffer
            RingBuffer_commit_read(input, 1);
            break;
        }
    }

    debug("LINE: %s", bdata(result));

    return result;
error:
    return NULL;
}
