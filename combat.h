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
void displayBattleOption();

float damage(float damage, int def);
void heal(float value, Character *character);


void apply_skill(int idx_skill, Turn_node *node, Character *target); 
void update_cooldowns (Turn_node *node); 
void TimeStrike(int id, Character *user, Character *target);

int random_Time_Strike(Character *character);

int isempty(Character *character);
int isfull(Character *character);
int peek(Character *character);
int pop(Character *character);
void push(Character *character, int data);

Turn_node* create_Tnode();
void init_Tnode(Turn_node *node, Character *character);

// Return adress of a queue
Turn_queue* create_Tqueue();
void init_Tqueue(Turn_queue *queue, Character *player, Character enemies[]);
void enqueue(Turn_queue *queue, Turn_node *node);
Turn_node *dequeue(Turn_queue *queue);

void display_enemies(Turn_queue *queue); 
void display_battle(Turn_queue *queue);

void display_skills(Turn_node *node);
void player_turn(Turn_node *node, Turn_queue *queue, Game_state *current_state);

bool combat(Character *player, Character enemies[], Game_state *current_state);
void enemy_skill_use(Turn_node *node, Turn_queue *queue, Game_state *current_state);
#endif