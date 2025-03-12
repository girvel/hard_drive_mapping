#include <string.h>
#include <stdbool.h>


typedef size_t (*HashFunction)(void *);

typedef struct {
    void *address;
    size_t key_size;
    size_t value_size;
    size_t capacity;
    size_t size;
    HashFunction hash;
    bool (*equal)(void *, void *);
} Map;

void map_init(Map *self, size_t key_size, size_t value_size, size_t capacity, HashFunction hash_function);
void map_free(Map *self);
void map_set(Map *self, void *key, void *value);
void *map_get(Map self, void *key, void *default_value);
