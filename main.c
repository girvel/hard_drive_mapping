#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "lib/hd.h"
#include "lib/list.h"


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

int main() {
    printf("  DYNAMIC ARRAY\n");

    List sample;
    list_init(&sample, sizeof(int));
    list_push_many(&sample, 4, (int[]){12, 24, 48, 96});
    printf("%i\n", *LIST_AT(&sample, int, 2));


    printf("\n  OWNER PROFILE\n");

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
        person_display(owner);
    }

    hd_unmap(owner);


    printf("\n  ALL PROFILES\n");

    const char *profiles_stats = STORAGE "profiles_stats";
    const char *profiles_content = STORAGE "profiles_content";

    is_empty = access(profiles_stats, F_OK) != 0;

    List *all_profiles = hd_map(profiles_stats, sizeof(List));
    // TODO! replace prev line by introducing Allocator.allocate?
    if (is_empty) {
        list_init(all_profiles, sizeof(Person));
    }

    HdAllocator profiles_allocator;
    hd_allocator_init(&profiles_allocator, profiles_content);
    all_profiles->allocator = (void *)&profiles_allocator;
    all_profiles->array = hd_map(profiles_content, all_profiles->capacity * all_profiles->item_size);

    list_push(all_profiles, &(Person) {
        .first_name = "Demo",
        .second_name = "Demovich",
        .last_name = "Demoviev",
        .birth_year = 1999 + all_profiles->size,
        .siblings_n = 0,
        .relationship_status = RelationshipStatus_Single,
    });

    for (size_t i = 0; i < all_profiles->size; i++) {
        person_display(LIST_AT(all_profiles, Person, i));
    }

    list_free(all_profiles);
    hd_unmap(all_profiles);

    return 0;
}
