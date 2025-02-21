#include <stdio.h>
#include "lib/hd.h"
#include <assert.h>

int main() {
    const size_t FILE_SIZE = 16;
    char *data = hd_map("/home/girvel/storage", FILE_SIZE);
    assert(data != NULL);

    printf("Incrementing\n");
    for (size_t i = 0; i < FILE_SIZE; i++) {
        data[i]++;
    }

    for (size_t i = 0; i < FILE_SIZE; i++) {
        printf("%i ", data[i]);
    }
    printf("\n");

    hd_unmap(data);

    return 0;
}
