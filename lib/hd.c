#include <sys/mman.h>
#include <sys/stat.h>
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

    return address;
}

Fat hd_map_all(const char *filename) {
    if (map_stats_size >= MAX_MAPPED_N) return (Fat) {0};

    int descriptor = open(filename, O_RDWR | O_CREAT, 00700);
    if (descriptor == -1) return (Fat) {0};

    struct stat file_stat;
    if (fstat(descriptor, &file_stat) == -1) return (Fat) {0};
    
    void* address = mmap(
        NULL, file_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0
    );
    if (address == NULL) return (Fat) {0};

    map_stats[map_stats_size] = (MappedMemory) {
        .descriptor = descriptor,
        .size = file_stat.st_size,
        .address = address,
    };
    map_stats_size++;

    return (Fat) {address, file_stat.st_size};
}

typedef struct {
    MappedMemory stat;
    off_t index;
} SearchResult;

SearchResult find_stat(void *address) {
    for (size_t i = 0; i < map_stats_size; i++) {
        MappedMemory stat = map_stats[i];
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
