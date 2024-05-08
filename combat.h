// Created 08/05/2024

#ifndef _COMBAT_H_

#include <character.h>

#define _COMBAT_H_
#define MAX_TURNS 10

// Queue with linkedlists
// Each node contains a character (Player, Enemy)
typedef struct Turn_node {
    Character character;
    Turn_node *next;
} Turn_node;

typedef struct turn_queue {  
    Turn_node *head;
    Turn_node *tail;
    int size;
    int num_turns;
} turn_queue;

#endif _COMBAT_H_