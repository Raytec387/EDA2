#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include "scenario.h"
#include "game.h"

// Function definitions 
void run(Game_state *currentState);
void start_game(Game_state *currentState);
void load_game(const char *file_name, Game_state *currentState);
void save_game(const char *file_name, Game_state *currentState);

#endif