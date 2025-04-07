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

size_t hash_size_t(void *value) {
    return *(size_t *)value;
}

void test_map_integers() {
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

size_t hash_str5(void *_str) {
    char *str = _str;
    char result = str[0];
    for (char i = 1; i < 5; i++) {
        result ^= str[(size_t) i] ^ i;
    }
    return result;
}

void test_map_strings() {
    Map words;
    map_init(&words, 5, sizeof(int8_t), 10, hash_str5);

    map_set(&words, "hell", REF(3));
    map_set(&words, "base", REF(9));
    map_set(&words, "oh", REF(-4));

    ASSERT(*(int8_t *)map_get(words, "hell", REF(-1)) == 3);
    ASSERT(*(int8_t *)map_get(words, "base", REF(-1)) == 9);
    ASSERT(*(int8_t *)map_get(words, "oh", REF(-1)) == -4);
}

void test_map_zero() {
    Map map;
    map_init(&map, sizeof(size_t), sizeof(size_t), 5, hash_size_t);

    map_set(&map, REF(0), REF(3));
    ASSERT(*(size_t *)map_get(map, REF(0), NULL) == 3);
}

int main() {
    return run_tests((Test[]) {
        test_list_usage,
        test_map_integers,
        test_map_strings,
        test_map_zero,
        NULL,
    });
}
