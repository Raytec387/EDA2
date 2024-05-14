#include "combat.h"

// Function to initialize a node struct
// Returns adress of a node
Turn_node* create_Tnode(Character *character) {
    Turn_node *new_node = malloc(sizeof(*new_node));
    new_node->character = *character;
    new_node->next = NULL;
    return new_node;
}

// Function to initialize a queue
// Return adress of a queue
Turn_queue* create_Tqueue() {
    Turn_queue *new_queue = malloc(sizeof(*new_queue));
    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->num_turns = 0;
    new_queue->size = 0;
    return new_queue;
}

// Function to enqueue node to a queue
void enqueue(Turn_queue *queue, Turn_node *node) {
    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    queue->size++;
}

// Function to dequeue node from a queue
// Returns the address of the dequeued node
Turn_node *dequeue(Turn_queue *queue) {
    Turn_node *temp = queue->head;
    if (queue->head == queue->head) {
        queue->head == NULL;
        queue->tail == NULL;
    } else {
        queue->head = queue->head->next;
    }
    queue->size--;
}
