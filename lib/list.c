#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "./list.h"

#include <stdio.h>


void list_init(List *self, size_t item_size) {
    self->array = NULL;
    self->item_size = item_size;
    self->size = 0;
    self->capacity = 0;
}

void list_push(List *self, void *value) {
    if (self->size >= self->capacity) {
        if (self->capacity == 0) self->capacity = 1;
        else self->capacity *= 2;

        self->array = realloc(self->array, self->capacity);
    }

    memcpy(self->array + self->item_size * self->size, value, self->item_size);
    self->size++;
}

void *list_at(List *self, size_t index) {
    return self->array + self->item_size * index;
}
