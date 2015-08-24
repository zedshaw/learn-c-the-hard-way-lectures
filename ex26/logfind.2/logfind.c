#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t MAX_LINE = 1024;

int scan_file(const char *filename, int search_len, char *search_for[])
{
    char *line = calloc(MAX_LINE, 1);
    FILE *file = fopen(filename, "r");
    char *found = NULL;
    int i = 0;

    check_mem(line);
    check(file, "Failed to open file: %s", filename);

    // read each line of the file and search that line for the contents
    while(fgets(line, MAX_LINE-1, file) != NULL && found == NULL) {
        for(i = 0; i < search_len && found == NULL; i++) {
            found = strcasestr(line, search_for[i]);
            if(found) {
                printf("%s\n", filename);
            }
        }
    }

    free(line);
    fclose(file);
    return 0;

error:
    if(line) free(line);
    if(file) fclose(file);

    return -1;
}


int main(int argc, char *argv[])
{
    check(argc > 1, "USAGE: logfind word word word");

    scan_file("logfind.c", argc, argv);

    return 0;

error:
    return 1;
}
