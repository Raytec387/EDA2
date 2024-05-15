// Created on 2024/4/24.
// This head will hold the data structure of the role(enemy or main character)
// And skill of main character
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "common.h"
#include "skill.h"

#define NAME_LENGTH 20
#define DESCRITPION_LENGTH 100
#define MAX_SKILL 4

// Data structure of character it holds the basic datas of the characters
typedef struct {
    char name[NAME_LENGTH];
    int atk;
    int hp;
    int hp_limit; 					// constant value, the max hp
    int def;
    Skill skill_array[MAX_SKILL];
    bool is_player;
} Character;

#endif

