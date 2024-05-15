// Created 08/05/2024

#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "common.h"
#include "character.h"

#define MAX_TURNS 10

// Queue with linkedlists
// Each node contains a character (Player, Enemy)
typedef struct Turn_node {
    Character character;
    bool is_last;
    Turn_node *next;
} Turn_node;

// Queue struct, contains information for num of turns and last and first element in queue
typedef struct Turn_queue {  
    Turn_node *head;
    Turn_node *tail;
    int size;
    int num_turns;
} Turn_queue;

// Combat Functions

Turn_node* create_Tnode(Character *character);

Turn_queue* create_Tqueue();

void enqueue(Turn_queue *queue, Turn_node *node);

Turn_node* dequeue(Turn_queue *queue);

#endif