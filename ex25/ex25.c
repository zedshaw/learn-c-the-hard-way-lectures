/**WARING: This code is fresh and potentially isn't correct yet.*/
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer) 
{
    *out_string = calloc(1, max_buffer+1);
    check_mem(*out_string);

    char *result = fgets(*out_string, max_buffer, stdin);
    check(result != NULL, "Input error.");

    return 0;

error:
    if(*out_string) free(*out_string);
    *out_string = NULL;
    return -1;
}

int read_int(long *out_int) 
{
    char *input = NULL;
    char *end = NULL;
    int rc = read_string(&input, MAX_DATA);
    check( 0 == rc, "Failed to read number.");

    *out_int = strtoi(input, &end, 10, 0, MAX_DATA, &rc);
    check( 0 != rc, "Failed to read number.");
    check((*end == '\0' || *end == '\n') &&
            *input != '\0', "Invaid number: %s", input);

    free(input);
    return 0;

error:
    if(input) free(input);
    return -1;
}

int read_scan(const char *fmt, ...) 
{
    //int i = 0;
    int rc = 0;
    long *out_int = NULL;
    char *out_char = NULL;
    char **out_string = NULL;
    int max_buffer = 0;

    va_list argp;
    va_start(argp, fmt);

    for (int i = 0; fmt[i] != '\0'; i++) {
        if(fmt[i] == '%') {
            i++;
            switch(fmt[i]) {
                case '\0':
                    sentinel("Invaid format, you ended with %%.");
                    break;

                case 'd':
                    out_int = va_arg(argp, long *);
                    rc = read_int(out_int);
                    check(0==rc, "Failed to read int.");
                    break;

                case 'c':
                    out_char = va_arg(argp, char *);
                    *out_char = fgetc(stdin);
                    break;

                case 's':
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    check(0 == rc, "Failed to read string.");
                    break;

                default:
                    sentinel("Invaid format.");
            }
        } else {
            fgetc(stdin);
        }
    
        check(!feof(stdin) && !ferror(stdin), "Input error.");
    }

    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}

int main(int argc, char *argv[])
{
    char *first_name = NULL;
    char initial = ' ';
    char *last_name = NULL;
    long age = 0;

    printf("What's your first name? ");
    int rc = read_scan("%s", MAX_DATA, &first_name);
    check(0==rc, "Failed first name.");

    printf("What's your initial? ");
    rc  = read_scan("%c\n", &initial);
    check(0==rc, "Failed initial.");

    printf("What's your last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(0==rc, "Failed last name.");

    printf("How old are you? ");
    rc = read_scan("%d", &age); //这里就是 %d 不要写成 %ld;
    check(0==rc, "Failed to read age.");

    printf("========== RESULTS ==============\n");
    printf("First Name: %s", first_name);
    printf("Initial: '%c'\n", initial);
    printf("Last Name: %s", last_name);
    printf("Age: %ld\n", age);

    free(first_name);
    free(last_name);
    return 0;
error:
    return -1;
}




