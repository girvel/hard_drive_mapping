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
};

void list_init(List *self, size_t item_size) {
    self->address = NULL;
    self->allocator = &ram_allocator;
    self->item_size = item_size;
    self->size = 0;
    self->capacity = 0;
}

void list_extend_exact(List *self, size_t capacity_increase) {
    self->capacity += capacity_increase;
    self->address = self->allocator->reallocate(
        self->allocator, self->address, self->capacity * self->item_size
    );
}

void list_extend(List *self, size_t minimal_capacity_increase) {
    size_t new_capacity = MAX(self->capacity, (size_t) 1);
    size_t minimal_new_capacity = self->capacity + minimal_capacity_increase;
    for (; new_capacity < minimal_new_capacity; new_capacity *= 2);
    list_extend_exact(self, new_capacity - self->capacity);
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
