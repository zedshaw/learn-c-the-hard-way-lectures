#include <lcthw/ringbuffer.h>
#include <lcthw/dbg.h>
#include <unistd.h>
#include <stdlib.h>
#include "statserve.h"
#include "net.h"
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

const int RB_SIZE = 1024 * 10;

int client_handler(int fd)
{
    int rc = 0;
    RingBuffer *sock_rb = RingBuffer_create(RB_SIZE);
    // child process

    while(read_some(sock_rb, fd, 1) != -1) {
        if(write_some(sock_rb, fd, 1) == -1) {
            log_info("Client closed.");
            break;
        }
    }

    rc = close(fd);
    check(rc != -1, "Failed to close fd.");

error: // fallthrough
    if(sock_rb) RingBuffer_destroy(sock_rb);
    exit(0);
}

void handle_sigchld(int sig) {
    while(waitpid(-1, NULL, WNOHANG) > 0) {
    }
}

int echo_server(const char *host, const char *port)
{
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(client_addr);
    int server_socket = 0;
    int client_fd = 0;
    int rc = 0;

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

        log_info("Client connected.");

        rc = fork();
        check(rc != -1, "Failed to fork!");

        if(rc == 0) {
            // in the child process
            close(server_socket);
            client_handler(client_fd);
        } else {
            // server process
            close(client_fd);
        }
    }

    return 0;

error:
    return -1;
}

