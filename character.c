// Created 14/05/2024

#include "character.h"

// All function related to character creation is in here

Character* create_new_character(char name[], Skill skill[]){
    Character *new_character = malloc(sizeof(*new_character));
    strncpy(new_character->name, name, MAX_STR_NAME);
    return new_character;
}




