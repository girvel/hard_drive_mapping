#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "./list.h"
#include "./math.h"


void list_init(List *self, size_t item_size) {
    self->address = NULL;
    self->item_size = item_size;
    self->size = 0;
    self->capacity = 0;
}

size_t _find_capacity(size_t prev_capacity, size_t size) {
    for (; prev_capacity < size; prev_capacity += MAX(1U, prev_capacity));
    return prev_capacity;
}

void list_extend_exact(List *self, size_t capacity_increase) {
    size_t old_capacity = self->capacity;
    self->capacity += capacity_increase;
    void *new_address = realloc(self->address, self->capacity * self->item_size);
    if (self->address != new_address && self->address != NULL) {
        memcpy(self->address, new_address, old_capacity);
    }
    self->address = new_address;
}

void list_extend(List *self, size_t minimal_capacity_increase) {
    list_extend_exact(
        self, _find_capacity(self->capacity, minimal_capacity_increase) - self->capacity
    );
}

void list_push(List *self, void *value) {
    if (self->size >= self->capacity) list_extend(self, 1);
    memcpy(self->address + self->item_size * self->size, value, self->item_size);
    self->size++;
}

void list_push_many(List *self, size_t count, void *values) {
    list_extend(self, count);

    for (size_t i = 0; i < count; i++) {
        list_push(self, values + self->item_size * i);
    }
}

void *list_at(List *self, size_t index) {
    return self->address + self->item_size * index;
}

void list_free(List *self) {
    free(self->address);
}
