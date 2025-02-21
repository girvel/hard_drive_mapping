#include <stdint.h>
#include <string.h>


typedef struct {
    void *array;
    size_t item_size;
    size_t size;
    size_t capacity;
} List;

void list_init(List *self, size_t item_size);
void list_extend_exact(List *self, size_t capacity_increase);
void list_extend(List *self, size_t minimal_capacity_increase);
void list_push(List *self, void *value);
void list_push_many(List *self, size_t count, void *values);
void *list_at(List *self, size_t index);

#define LIST_PUSH(SELF, TYPE, VALUE) list_push((SELF), &(struct {TYPE _;}){(VALUE)})
#define LIST_AT(SELF, TYPE, INDEX) (TYPE *)list_at((SELF), (INDEX))
