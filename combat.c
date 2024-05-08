#include <common.h>
#include <combat.h>

Turn_node* create_Tnode(Character *character) {
    Turn_node* new_node = malloc(sizeof(*new_node));
    new_node->character = *character;
    new_node->next = NULL;
}