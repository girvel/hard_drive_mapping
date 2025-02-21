#include <stdint.h>
#include <string.h>


typedef struct {
    void *array;
    size_t item_size;
    size_t size;
    size_t capacity;
} List;

void list_init(List *self, size_t item_size);
void list_push(List *self, void *value);
void *list_at(List *self, size_t index);

