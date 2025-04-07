#include "./map.h"
#include <stdlib.h>
#include <stdbool.h>
#include "./macros.h"

// TODO item_size
// TODO use eq
// TODO allow 0 as a key
// TODO dynamic resize
// TODO iteration


bool _is_zero(void *address, size_t size) {
    char *typed_address = address;
    for (size_t i = 0; i < size; i++) {
        if (typed_address[i] != 0) return false;
    }

    return true;
}

void map_init(Map *self, size_t key_size, size_t value_size, size_t capacity, HashFunction hash_function) {
    self->key_size = key_size;
    self->value_size = value_size;
    self->capacity = capacity;
    size_t memory_length = (self->key_size + self->value_size)
        * self->capacity;
    self->address = malloc(memory_length);
    memset(self->address, 0, memory_length);
    self->size = 0;
    self->hash = hash_function;
}

void map_free(Map *self) {
    if (self->address == NULL) return;
    free(self->address);
    self->address = NULL;
    self->capacity = 0;
    self->size = 0;
}

void map_set(Map *self, void *key, void *value) {
    size_t hash = self->hash(key);
    size_t step = self->key_size + self->value_size;
    size_t offset = hash % self->capacity;

    void *key_stored;
    while (true) {
        key_stored = self->address + offset * step;
        if (
            _is_zero(key_stored, self->key_size) ||
            memcmp(key_stored, key, self->key_size) == 0
        ) break;

        offset = (offset + 1) % self->capacity;
    }

    if (key_stored == NULL) self->size++;
    memcpy(key_stored, key, self->key_size);
    memcpy(key_stored + self->key_size, value, self->value_size);
}

void *map_get(Map self, void *key, void *default_value) {
    if (key == 0) return default_value;

    size_t hash = self.hash(key);
    size_t step = self.key_size + self.value_size;
    size_t initial_offset = hash % self.capacity;
    size_t offset = initial_offset;

    while (memcmp(self.address + offset * step, key, self.key_size) != 0) {
        offset = (offset + 1) % self.capacity;
        if (offset == initial_offset) {
            return default_value;
        }
    }

    return self.address + offset * step + self.key_size;
}

