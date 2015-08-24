#include <stdio.h>
#include <lcthw/dbg.h>
#include "statserve.h"
#include "net.h"


int main(int argc, char *argv[])
{
    check(argc == 3, "USAGE: statserve host port");

    const char *host = argv[1];
    const char *port = argv[2];

    check(echo_server(host, port), "Failed to run the echo server.");

    return 0;

error:
    
    return 1;
}
