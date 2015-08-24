#include "dbg.h"


int main(int argc, char *argv[])
{
    check(argc > 2, "USAGE: logfind word word word");

    return 0;

error:
    return 1;
}
