#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define FILE_SIZE 16

typedef struct {
    int x;
    int y;
    int z;
    char name[16];
} Point;

int main() {
    int descriptor = open("storage", O_RDWR | O_CREAT);
    assert(descriptor != -1 && "unable to open the file");
    assert(ftruncate(descriptor, FILE_SIZE) != -1 && "unable to truncate file");

    char *data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor, 0);
    assert(data != NULL && "mmap returned NULL");

    printf("Incrementing\n");
    for (size_t i = 0; i < FILE_SIZE; i++) {
        data[i]++;
    }

    for (size_t i = 0; i < FILE_SIZE; i++) {
        printf("%i ", data[i]);
    }
    printf("\n");

    munmap(data, FILE_SIZE);
    close(descriptor);

    return 0;
}
