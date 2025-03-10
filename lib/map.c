#include "./map.h"
#include <stdlib.h>
#include "./math.h"

// TODO memcpy
// TODO item_size


void map_set(Map *self, size_t key, size_t value) {
    size_t offset = 2 * (key % (self->capacity / 2));
    self->address[offset] = key;
    self->address[offset + 1] = value;
}

size_t map_get(Map self, size_t key, size_t default_value) {
    size_t offset = 2 * (key % (self.capacity / 2));
    size_t key_stored = self.address[offset];
    if (key_stored != key) {
        return default_value;
    }
    return self.address[offset + 1];
}

