// Created by zihao zhou on 2024/4/24.
// This head will hold the data structure of the role(enemy or main character)
#include "common.h"

typedef struct {
    char name[NAME_LENGTH];
    int atk;
    int hp;
    int def;
    int skill_array[MAX_SKILL];
}Main_character;

typedef struct {
    char name[NAME_LENGTH];
    int atk;
    int hp;
    int def;
}Enemy;

// Main struct handling skills functionality
typedef struct Skills {
    char name[NAME_LENGTH];
    int attack;
    int type;
    int uses;
    int cooldown;
    char desc[DESCRITPION_LENGTH];
} Skills;

#ifndef CHARACTER_H
#define CHARACTER_H

#endif //CHARACTER_H

