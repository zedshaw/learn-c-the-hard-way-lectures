#ifndef _ex22_h
#define _ex22_h

// Makes the_size available in other .c files
struct State {
    int the_size;
    int the_age;
};
int get_age(struct State *state);
void set_age(struct State *state, int age);
int get_size(struct State *state);
void set_size(struct State *state, int size);
double update_ratio(double ratio);
void print_size(struct State *state);
#endif
