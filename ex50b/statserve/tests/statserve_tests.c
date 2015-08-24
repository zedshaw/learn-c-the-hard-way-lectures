#include "minunit.h"
#include <dlfcn.h>
#include "statserve.h"
#include <lcthw/bstrlib.h>
#include <lcthw/ringbuffer.h>
#include <assert.h>

typedef struct LineTest {
    char *line;
    bstring result;
    char *description;
} LineTest;

int attempt_line(LineTest test)
{
    int rc = -1;
    bstring result = NULL;

    bstring line = bfromcstr(test.line);
    RingBuffer *send_rb = RingBuffer_create(1024);

    rc = parse_line(line, send_rb);
    check(rc == 0, "Failed to parse line.");

    result = RingBuffer_get_all(send_rb);
    check(result != NULL, "Ring buffer empty.");
    check(biseq(result, test.result), "Got the wrong output: %s expected %s",
            bdata(result), bdata(test.result));

    bdestroy(line);
    RingBuffer_destroy(send_rb);
    return 1; // using 1 for tests
error:
  
    log_err("Failed to process test %s: got %s", test.line, bdata(result));
    if(line) bdestroy(line);
    if(send_rb) RingBuffer_destroy(send_rb);
    return 0;
}


int run_test_lines(LineTest *tests, int count)
{
    int i = 0;

    for(i = 0; i < count; i++) {
        check(attempt_line(tests[i]), "Failed to run %s", tests[i].description);
    }

    return 1;
error:
    return 0;
}

int fake_command(Command *cmd, RingBuffer *send_rb, bstring path)
{
    check(cmd != NULL, "Bad cmd.");
    check(cmd->path != NULL, "Bad path.");
    check(send_rb != NULL, "Bad send_rb.");
    check(path != NULL, "Bad path given.");

    return 0;
error:
    return -1;
}

char *test_path_parsing()
{
    struct bstrList *result = NULL;
    struct tagbstring slash = bsStatic("/");
    struct tagbstring logins_zed = bsStatic("/logins/zed");
    struct tagbstring command_name = bsStatic("dump");
    RingBuffer *send_rb = RingBuffer_create(1024);
    struct bstrList *path = bsplits(&logins_zed, &slash);
    int rc = 0;

    Command fake = {
        .command = &command_name,
        .name = &logins_zed,
        .number = NULL,
        .handler = fake_command,
        .path = path
    };

    result = parse_name(&logins_zed);
    mu_assert(result != NULL, "Failed to parse /logins/zed");

    rc = scan_paths(&fake, send_rb); 
    mu_assert(rc != -1, "scan_paths failed.");

    return NULL;
}

char *test_create()
{
    LineTest tests[] = {
        {.line = "create /zed 100", .result = &OK, .description = "create zed failed"},
        {.line = "create /joe 100", .result = &OK, .description = "create joe failed"},

    };

    mu_assert(run_test_lines(tests, 2), "Failed to run create tests.");

    return NULL;
}

char *test_sample()
{
    struct tagbstring sample1 = bsStatic("100.000000\n");

    LineTest tests[] = {
        {.line = "sample /zed 100", .result = &sample1, .description = "sample zed failed."}
    };

    mu_assert(run_test_lines(tests, 1), "Failed to run sample tests.");

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    int rc = setup_data_store();
    mu_assert(rc == 0, "Failed to setup the data store.");

    mu_run_test(test_create);
    mu_run_test(test_sample);
    mu_run_test(test_path_parsing);

    return NULL;
}

RUN_TESTS(all_tests);
