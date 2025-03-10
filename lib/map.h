#include <string.h>


typedef struct {
    size_t *address;
    size_t capacity;
    size_t size;
} Map;

void map_set(Map *self, size_t key, size_t value);
size_t map_get(Map self, size_t key, size_t default_value);
