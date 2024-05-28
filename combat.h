// Created 08/05/2024

#ifndef COMBAT_H
#define COMBAT_H

#include "common.h"
#include "skill.h"
#include "character.h"
#include "game.h"

#define MAX_TURNS 10
#define MAX_ENEMIES 3
#define USE_ATTACK 0
#define USE_SKILL 1
#define USE_TIME_STRIKE 2

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

int damage(float damage, int def);

void heal(float value, Character *character);

void apply_active_effect(Character *character);

void apply_skill(Skill *skill, Character *user, Character *target, Game_state *currentState);

void target_skill(Skill *skill, Character *user, Character *characters, int target_index);

void TimeStrike(int id, Character *user, Character *target, Game_state *current_state);

int random_Time_Strike(Game_state *current_state);

int isempty(Game_state *current_state);
int isfull(Game_state *current_state);
int peek(Game_state *current_state);
int pop(Game_state *current_state);
void push(Game_state *current_state, int data);

Turn_node* create_Tnode(Character *character);

Turn_queue* create_Tqueue();

void enqueue(Turn_queue *queue, Turn_node *node);

Turn_node *dequeue(Turn_queue *queue);

int combat(Character *player, Character *characters);

void enemy_skill_use(Character *enemy, Character *player);

#endif