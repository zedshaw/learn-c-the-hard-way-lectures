#include "minunit.h"

char *test_dlopen(int stuff)
{
    return NULL;
}

char *test_functions()
{

    return NULL;
}

char *test_failures()
{

    return NULL;
}

char *test_dlclose()
{

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_dlopen);
    mu_run_test(test_functions);
    mu_run_test(test_failures);
    mu_run_test(test_dlclose);

    return NULL;
}

RUN_TESTS(all_tests);
