#include "./map.h"
#include <stdlib.h>
#include "./math.h"

// TODO memcpy
// TODO item_size


void map_set(Map *self, size_t key, size_t value) {
    size_t offset = 2 * (key % self->capacity);
    while (self->address[offset] != key && self->address[offset] != 0) {
        offset = (offset + 2) % self->capacity;
    }

    if (self->address[offset] == 0) self->size++;
    self->address[offset] = key;
    self->address[offset + 1] = value;
}

size_t map_get(Map self, size_t key, size_t default_value) {
    if (key == 0) return default_value;

    size_t initial_offset = 2 * (key % self.capacity);
    size_t offset = initial_offset;
    while (self.address[offset] != key) {
        offset = (offset + 2) % (self.capacity);
        if (offset == initial_offset) {
            return default_value;
        }
    }

    size_t key_stored = self.address[offset];
    if (key_stored != key) {
        return default_value;
    }
    return self.address[offset + 1];
}

