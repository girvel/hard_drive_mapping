#include <stdio.h>

typedef void (*Test)();

void _test_fail();
char *_test_message();
int run_tests(Test *tests);

#define ASSERT(CONDITION) \
    if (!(CONDITION)) { \
        _test_fail(); \
        snprintf(_test_message(), TEST_MAX_MESSAGE_SIZE, "%s:%i", __func__, __LINE__); \
        return; \
    }

#define TEST_MAX_MESSAGE_SIZE 1024
