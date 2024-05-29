// Created on 2024/4/24.
// This head will hold the data structure of the role(enemy or main character)
// And skill of main character
#ifndef CHARACTER_H
#define CHARACTER_H

#include "skill.h"
#include "common.h"

// Times strike stack, record the used skill in the stack, randomly choose one
typedef struct {
    int top;
    int stack[MAX_STACK_SIZE];
    bool time_strike_use;
} Time_strike_stack;

// Data structure of character it holds the basic datas of the characters
typedef struct Character{
    char name[NAME_LENGTH];
    int atk;
    float hp;
    float hp_limit; 					// constant value, the max hp
    int def;
    Skill skill_array[MAX_SKILL];
    bool is_player;
    Effect active_effects[MAX_ACTIVE_EFFECTS]; // increasing ATK/DEF for 2 turn
    AbilityTracker *tracker;
    Time_strike_stack ability_stack;
} Character;

// Create a new enemy
Character* create_new_enemy();

// initialize the enemy's data
void initialize_enemy(int i, Character *enemy);

// initialize the enemy's array(number of enemy in the battle)
void initialize_enemies_array(int id_enemy,Character *enemy[],int number_of_enemies);

// initialize the main character's data
void initialize_main_character(Character *player);

#endif

