#include <string.h>
#include <stdbool.h>


typedef struct {
    void *address;
    size_t key_size;
    size_t value_size;
    size_t capacity;
    size_t size;
    size_t (*hash)(void *);
    bool (*equal)(void *, void *);
} Map;

void map_set(Map *self, void *key, void *value);
void *map_get(Map self, void *key, void *default_value);
