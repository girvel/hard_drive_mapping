#pragma once

#include <aio.h>

typedef struct {
    int descriptor;
    size_t size;
    void *address;
} MappedMemory;

void *hd_map(const char *filename, size_t size);
void hd_unmap(void *address);

typedef struct {
    void *(*reallocate)(void *, void *, size_t);
    void (*free)(void *, void *);
    const char *filename;
} HdAllocator;
