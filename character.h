// Created on 2024/4/24.
// This head will hold the data structure of the role(enemy or main character)
// And skill of main character
#ifndef CHARACTER_H
#define CHARACTER_H

#include "common.h"
#include "skill.h"

// Data structure of character it holds the basic datas of the characters
typedef struct {
    char name[NAME_LENGTH];
    int atk;
    int hp;
    int hp_limit; 					// constant value, the max hp
    int def;
    Skill skill_array[MAX_SKILL];
    bool is_player;
    Effect active_effects[MAX_ACTIVE_EFFECTS];
} Character;

#endif

