// Created on 2024/4/24.
// This head will hold the data structure of the role(enemy or main character)
// And skill of main character
#ifndef CHARACTER_H
#define CHARACTER_H

#include "common.h"

// Data structure of main character it holds the basic datas of the characters
// Such as atk/hp/def, skills, the name
typedef struct {
    char name[NAME_LENGTH];
    int atk;
    int hp;
    int def;
    Skills skill_array[MAX_SKILL];
    bool is_player;
} Character;

// Main struct handling skills functionality
typedef struct Skills {
    char name[NAME_LENGTH];
    int value;
    int type;
    int cooldown;
    char desc[DESCRITPION_LENGTH];
} Skills;

#endif

