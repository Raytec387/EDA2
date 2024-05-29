#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include "scenario.h"
#include "game.h"

// Function definitions 
void run(Game_state *currentState);                                 // Run the game/menu
void start_game(Game_state *currentState);                          // Start a new game, start by charpter I
void load_game(const char *file_name, Game_state *currentState);    // Load game states from save.json
void save_game(const char *file_name, Game_state *currentState);    // Save game states to save.json

#endif