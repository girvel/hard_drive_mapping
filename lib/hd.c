#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include "./hd.h"

#include "stdio.h"


#define MAX_MAPPED_N 64
MappedMemory map_stats[MAX_MAPPED_N];
size_t map_stats_size = 0;

void *hd_map(const char *filename, size_t size) {
    if (map_stats_size >= MAX_MAPPED_N) return NULL;

    int descriptor = open(filename, O_RDWR | O_CREAT, 00700);
    if (descriptor == -1) return NULL;
    if (ftruncate(descriptor, size) == -1) return NULL;

    void* address = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0);
    if (address == NULL) return NULL;

    map_stats[map_stats_size] = (MappedMemory) {
        .descriptor = descriptor,
        .size = size,
        .address = address,
    };
    map_stats_size++;
    printf("%s is remembered as %p\n", filename, address);

    return address;
}

typedef struct {
    MappedMemory stat;
    size_t index;
} SearchResult;

SearchResult find_stat(void *address) {
    printf("searching for %p\n", address);
    for (size_t i = 0; i < map_stats_size; i++) {
        MappedMemory stat = map_stats[i];
        printf("found %p\n", stat.address);
        if (stat.address == address) return (SearchResult) {
            .stat = stat,
            .index = i,
        };
    }
    return (SearchResult) {.index = -1};
}

void hd_unmap(void *address) {
    SearchResult match = find_stat(address);
    munmap(address, match.stat.size);
    close(match.stat.descriptor);
    map_stats[match.index] = map_stats[map_stats_size];
    map_stats_size--;
}

void *_hd_reallocate(void *self, void *prev, size_t size) {
    HdAllocator *self_typed = self;
    if (prev == NULL) return hd_map(self_typed->filename, size);

    SearchResult match = find_stat(prev);
    if (match.index == -1) return hd_map(self_typed->filename, size);

    ftruncate(match.stat.descriptor, size);
    munmap(match.stat.address, match.stat.size);
    match.stat.size = size;
    match.stat.address = mmap(
        prev, size, PROT_READ | PROT_WRITE, MAP_SHARED, match.stat.descriptor, 0
    );
    return match.stat.address;
}

void _hd_free(void *self, void *address) {
    (void)self;
    hd_unmap(address);
}

void hd_allocator_init(HdAllocator *self, const char *filename) {
    self->reallocate = _hd_reallocate;
    self->free = _hd_free;
    self->filename = filename;
}
