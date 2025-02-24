#pragma once

#include <aio.h>
#include "./memory.h"

typedef struct {
    int descriptor;
    size_t size;
    void *address;
} MappedMemory;

void *hd_map(const char *filename, size_t size);
Fat hd_map_all(const char *filename);
void hd_unmap(void *address);

typedef struct {
    void *(*reallocate)(void *, void *, size_t);
    void (*free)(void *, void *);
    const char *filename;
} HdAllocator;

void hd_allocator_init(HdAllocator *self, const char *filename);
