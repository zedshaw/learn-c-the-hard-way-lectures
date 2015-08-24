#include <stdio.h>
#include "ex22.h"
#include "dbg.h"

int get_age(struct State *state)
{
    return state->the_age;
}

void set_age(struct State *state, int age)
{
   state->the_age = age; 
}

double update_ratio(double new_ratio)
{
    static double ratio = 1.0;

    double old_ratio = ratio;
    ratio = new_ratio;

    return old_ratio;
}

void print_size()
{
    log_info("I think size is: %d", THE_SIZE);
}
