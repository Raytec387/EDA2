// Created 14/05/2024

#include "character.h"

// All function related to character creation is in here

Character* create_new_character(char name[], Skills skills[]){
    Character *new_character = malloc(sizeof(*new_character));
    *new_character->name = name;
    return new_character;
}


