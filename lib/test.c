#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "test.h"


bool _last_test_status;
char _last_test_message[TEST_MAX_MESSAGE_SIZE];

void _test_fail() {
    _last_test_status = false;
}

char *_test_message() {
    return _last_test_message;
}

int run_tests(Test *tests) {
    bool tests_failed = false;
    for (size_t i = 0; tests[i] != NULL; i++) {
        _last_test_status = true;
        tests[i]();
        if (!_last_test_status) {
            if (!tests_failed) {
                tests_failed = true;
                printf("----- FAILED: -----\n");
            }
            printf("%s\n", _last_test_message);
        }
    }
    return tests_failed ? 1 : 0;
}
