#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "./hd.h"

#define MAX_MAPPED_N 64
MappedMemory map_stats[MAX_MAPPED_N];
size_t map_stats_size = 0;

void *hd_map(const char *filename, size_t size) {
    if (map_stats_size == MAX_MAPPED_N) return NULL;

    int descriptor = open(filename, O_RDWR | O_CREAT);
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

void hd_unmap(void *address) {
    for (size_t i = 0; i < map_stats_size; i++) {
        MappedMemory stat = map_stats[i];
        if (stat.address == address) {
            munmap(address, stat.size);
            close(stat.descriptor);
            map_stats[i] = map_stats[map_stats_size];
            map_stats_size--;
            return;
        }
    }
}
