// Created 08/05/2024

#ifndef COMBAT_H
#define COMBAT_H

#include "common.h"
#include "skill.h"
#include "character.h"
#include "game.h"

#define MAX_TURNS 10
#define MAX_ENEMIES 3
#define USE_ATTACK 1
#define USE_SKILL 2
#define USE_TIME_STRIKE 3

// Queue with linkedlists
// Each node contains a character (Player, Enemy)
// Keeps track of available Skill
typedef struct Turn_node {
    Character *character;
    Skill *available_Skill[MAX_SKILL];
    int num_skill; 
    bool is_last;
    struct Turn_node *next;
} Turn_node;

// Queue struct, contains information for num of turns and last and first element in queue
typedef struct Turn_queue {  
    Turn_node *head;
    Turn_node *tail;
    Character *player;
    Character *enemies[MAX_ENEMIES];
    int size;
    int num_turns;
} Turn_queue;

// Combat Functions

#endif