#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "lib/list.h"
#include "lib/map.h"


bool is_simple(int n) {
    if (n < 2) return false;

    double end = sqrt(n);
    for (size_t d = 2; d <= end; d++) {
        if (n % d == 0) return false;
    }

    return true;
}

// void visualize_map(Map map) {
//     printf("size: %zu, capacity: %zu\n\n", map.size, map.capacity);
//     printf("| offst | key   | value |\n");
//     printf("| ----- | ----- | ----- |\n");
// 
//     size_t step = map.key_size + map.value_size;
//     for (size_t i = 0; i < map.capacity; i++) {
//         printf(
//             "| %zu \t| %zu \t| %zu \t|\n",
//             i,
//             map.address[step * i],
//             map.address[step * i + map.key_size]
//         );
//     }
// }

size_t hash_size_t(void *value) {
    return *(size_t *)value;
}

int main() {
    printf("  DYNAMIC ARRAY\n");

    List sample;
    list_init(&sample, sizeof(int));

    for (int n = 0; n < 10000; n++) {
        if (is_simple(n)) {
            list_push(&sample, &n);
        }
    }

    for (size_t i = 0; i < sample.capacity; i++) {
        printf("%i ", *LIST_AT(&sample, int, i));
    }
    printf("\n\ncapacity: %zu, size: %zu\n", sample.capacity, sample.size);

    list_free(&sample);


    printf("  HASHMAP\n");

    Map simple_numbers;
    simple_numbers.capacity = 50;
    simple_numbers.key_size = sizeof(size_t);
    simple_numbers.value_size = sizeof(size_t);
    size_t memory_length = (simple_numbers.key_size + simple_numbers.value_size)
        * simple_numbers.capacity;
    simple_numbers.address = malloc(memory_length);
    memset(simple_numbers.address, 0, memory_length);
    simple_numbers.size = 0;
    simple_numbers.hash = hash_size_t;

    size_t counter = 1;
    for (size_t n = 0; n < 97; n++) {
        if (is_simple(n)) {
            map_set(&simple_numbers, &n, &counter);
            counter++;
        }
    }

    printf("%zu\n", *(size_t *)map_get(
        simple_numbers,
        &(struct {size_t _;}){(53)},
        &(struct {size_t _;}){(0)}
    ));

    // visualize_map(simple_numbers);

    return 0;
}
