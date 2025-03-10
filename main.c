#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "lib/list.h"


typedef enum {
   RelationshipStatus_Single,
   RelationshipStatus_Dating,
   RelationshipStatus_Married,
} RelationshipStatus;

const char *status_to_string(RelationshipStatus base) {
    switch (base) {
        case RelationshipStatus_Single: return "single";
        case RelationshipStatus_Dating: return "dating";
        case RelationshipStatus_Married: return "married";
    }
    assert(false && "unreachable");
}

RelationshipStatus status_from_string(const char *base) {
    if (strcmp(base, "single") == 0) return RelationshipStatus_Single;
    if (strcmp(base, "dating") == 0) return RelationshipStatus_Dating;
    if (strcmp(base, "married") == 0) return RelationshipStatus_Married;
    return -1;
}

typedef struct {
    char first_name[16];
    char second_name[16];
    char last_name[16];
    int birth_year;
    int siblings_n;
    RelationshipStatus relationship_status;
} Person;

void person_display(Person *self) {
    printf("%s %s %s, born %i, %i siblings, %s\n",
        self->first_name, self->second_name, self->last_name,
        self->birth_year, self->siblings_n, status_to_string(self->relationship_status)
    );
}

bool is_simple(int n) {
    double end = sqrt(n);
    for (size_t d = 2; d <= end; d++) {
        if (n % d == 0) return false;
    }

    return true;
}

int main() {
    printf("  DYNAMIC ARRAY\n");

    List sample;
    list_init(&sample, sizeof(int));

    for (int n = 0; n < 10000; n++) {
        if (is_simple(n)) {
            list_push(&sample, &n);
        }
    }

    for (size_t i = 0; i < sample.capacity; i++) {
        printf("%i ", *LIST_AT(&sample, int, i));
    }
    printf("\n\ncapacity: %zu, size: %zu\n", sample.capacity, sample.size);

    list_free(&sample);

    return 0;
}
