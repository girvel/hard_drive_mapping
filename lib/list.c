#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "./list.h"
#include "./math.h"


void *_ram_reallocate(void *self, void *prev, size_t size) {
    (void) self;
    return realloc(prev, size);
}

void _ram_free(void *self, void *address) {
    (void) self;
    free(address);
}

Allocator ram_allocator = {
    .reallocate = _ram_reallocate,
    .free = _ram_free,
};  // TODO! move to memory.c

void list_init(List *self, size_t item_size) {
    self->address = NULL;
    self->allocator = &ram_allocator;
    self->item_size = item_size;
    self->size = 0;
    self->capacity = 0;
}

size_t _find_capacity(size_t prev_capacity, size_t size) {
    prev_capacity = MAX(1U, prev_capacity);
    for (; prev_capacity < size; prev_capacity *= 2);
    return prev_capacity;
}

void list_init_owning(List *self, size_t item_size, Fat memory, void *allocator) {
    self->address = memory.address;
    self->allocator = allocator == NULL ? &ram_allocator : allocator;
    self->item_size = item_size;
    self->size = memory.size;
    self->capacity = _find_capacity(0, memory.size / item_size);
}

void list_extend_exact(List *self, size_t capacity_increase) {
    size_t old_capacity = self->capacity;
    self->capacity += capacity_increase;
    void *new_address = self->allocator->reallocate(
        self->allocator, self->address, self->capacity * self->item_size
    );
    if (self->address != new_address && self->address != NULL) {
        memcpy(self->address, new_address, old_capacity);
    }
    self->address = new_address;
}

void list_extend(List *self, size_t minimal_capacity_increase) {
    list_extend_exact(self, _find_capacity(self->capacity, minimal_capacity_increase));
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
    self->allocator->free(self->allocator, self->address);
}
