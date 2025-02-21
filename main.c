#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>

#define FILE_SIZE 16

int main() {
    // int descriptor = open("storage", O_RDWR | O_CREAT);
    // assert(descriptor != -1 && "unable to open the file");

    // char *data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, descriptor, 0);
    // assert(data != NULL && "mmap returned NULL");
    
    char data[FILE_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    for (size_t i = 0; i < FILE_SIZE; i++) {
        printf("%i ", data[i]);
    }
    printf("\n");
    return 0;
}
