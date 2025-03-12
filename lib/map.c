#include "./map.h"
#include <stdlib.h>
#include <stdbool.h>
#include "./macros.h"

// TODO memcpy
// TODO item_size
// TODO


bool _is_zero(void *address, size_t size) {
    char *typed_address = address;
    for (size_t i = 0; i < size; i++) {
        if (typed_address[i] != 0) return false;
    }

    return true;
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

