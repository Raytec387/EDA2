// Created on 2024/4/24.
// This head will hold the data structure of the role(enemy or main character)
// And skill of main character
#ifndef CHARACTER_H
#define CHARACTER_H

#include "skill.h"
#include "common.h"

// Data structure of character it holds the basic datas of the characters
typedef struct Character{
    char name[NAME_LENGTH];
    int atk;
    float hp;
    float hp_limit; 					// constant value, the max hp
    int def;
    Skill skill_array[MAX_SKILL];
    bool is_player;
    Effect active_effects[MAX_ACTIVE_EFFECTS];
} Character;

Character* create_new_enemy();

void initialize_enemy(Game_state *currentState,Character *enemy);

void initialize_enemies_array(Game_state *currentState,Character *enemy[],int number_of_enemies);

void initialize_main_character(Game_state *currentState);
#endif

