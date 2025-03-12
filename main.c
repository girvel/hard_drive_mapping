#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "lib/list.h"
#include "lib/map.h"
#include "lib/test.h"
#include "lib/macros.h"


bool is_simple(int n) {
    if (n < 2) return false;

    double end = sqrt(n);
    for (size_t d = 2; d <= end; d++) {
        if (n % d == 0) return false;
    }

    return true;
}

void visualize_map(Map map) {
    printf("size: %zu, capacity: %zu\n\n", map.size, map.capacity);
    printf("key_size: %zu, value_size: %zu\n\n", map.key_size, map.value_size);

    size_t step = map.key_size + map.value_size;
    for (size_t i = 0; i < map.capacity; i++) {
        size_t *entry = map.address + i * step;
        printf("| %zu \t| %zu \t| %zu |\n", i, 0[entry], 1[entry]);
    }
}

size_t hash_size_t(void *value) {
    return *(size_t *)value;
}

void test_list_usage() {
    const size_t size = 1000;

    List sample;
    list_init(&sample, sizeof(int));
    for (int i = 0; i < (int) size; i++) {
        list_push(&sample, &i);
    }

    ASSERT(sample.size == size);
    ASSERT(sample.capacity == 1024);

    int sum = 0;
    for (size_t i = 0; i < sample.size; i++) {
        sum += i;
    }

    ASSERT(sum == 499500);

    list_free(&sample);
}

void test_map() {
    Map simple_numbers;
    map_init(&simple_numbers, sizeof(size_t), sizeof(size_t), 50, hash_size_t);

    size_t counter = 1;
    for (size_t n = 0; n < 100; n++) {
        if (is_simple(n)) {
            map_set(&simple_numbers, &n, &counter);
            counter++;
        }
    }

    counter = 1;
    for (size_t n = 0; n < 100; n++) {
        if (is_simple(n)) {
            ASSERT(*(size_t *)map_get(simple_numbers, &n, REF(0)) == counter);
            counter++;
        }
    }

    map_free(&simple_numbers);
}

int main() {
    return run_tests((Test[]) {
        test_list_usage,
        test_map,
        NULL,
    });
}
