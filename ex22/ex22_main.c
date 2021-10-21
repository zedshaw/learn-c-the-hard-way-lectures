#include "ex22.h"
#include "dbg.h"
#include <stdlib.h>

const char *MY_NAME = "Sam Olembo";
struct State state = { .the_size=2, .the_age=3 };
//struct State *state = malloc(sizeof(my_state));
void scope_demo(int count)
{
    log_info("count is %d", count);
    if (count > 10) {
        int i = 100; // BUGS!!
        log_info("count in this scope is %d", i);
    }
    log_info("count is at exit: %d", count);
    count = 3000;
    log_info("count after assign: %d", count);
}

int main(int argc, char *argv[])
{
    log_info("My name: %s, age: %d", MY_NAME, get_age(&state));
    set_age(&state, 100);
    log_info("My age is now: %d", get_age(&state));
    log_info("THE_SIZE is: %d", get_size(&state));
    print_size(&state);
    log_info("THE SIZE is now: %d", get_size(&state));
    set_size(&state, 9);
    log_info("THE SIZE is now: %d", get_size(&state));
    print_size(&state);

    log_info("Ratio at first: %f", update_ratio(2.0));
    log_info("Ratio again: %f", update_ratio(10.0));
    log_info("Ratio once more: %f", update_ratio(300.0));

    int count = 4;
    scope_demo(count);
    scope_demo(count * 20);
    log_info("count after calling scope_demo: %d", count);
    //free(state);
    return 0;
}
