#include <stdio.h>
#include <unistd.h>
#include "lib/hd.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define STORAGE "/var/cstorage/"

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

typedef struct {
    char first_name[16];
    char second_name[16];
    char last_name[16];
    int birth_year;
    int siblings_n;
    RelationshipStatus relationship_status;
} Person;

int main() {
    const char *owner_profile = STORAGE "owner_profile";
    bool is_empty = access(owner_profile, F_OK) != 0;
    Person *owner = hd_map(owner_profile, sizeof(Person));

    if (is_empty) {
        strcpy(owner->first_name, "Nikita");
        strcpy(owner->second_name, "Sergeyevich");
        strcpy(owner->last_name, "Dobrynin");
        owner->birth_year = 2001;
        owner->siblings_n = 1;
        owner->relationship_status = RelationshipStatus_Dating;
    } else {
        printf("%s %s %s, born %i, %i siblings, %s\n",
            owner->first_name, owner->second_name, owner->last_name,
            owner->birth_year, owner->siblings_n, status_to_string(owner->relationship_status)
        );
    }

    return 0;
}
