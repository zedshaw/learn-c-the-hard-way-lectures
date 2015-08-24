#include "minunit.h"
#include <statserve.h>


char *test_statserve()
{

    // mu_assert(echo_server("127.0.0.1", "7899") == 0, "Failed to start echo server.");

    return NULL;
}


char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_statserve);

    return NULL;
}

RUN_TESTS(all_tests);
