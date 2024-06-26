#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "character.h"
#include "skill.h"

// Keep tracking the currunt game state
typedef struct {
    // keep track of current scenario
    int currentScenarioId;
    Character character; // Keeps track of the current character
} Game_state;

// Create a new game state and copy game state
Game_state* make_game_state();
Game_state* copy_game_state(Game_state *game_state);

// free game state once finished the game
void free_game_state(Game_state *game_state);

// Load/Save the data from/to save.json
cJSON *create_json_from_game_state(Game_state *state);
void parse_game_state_from_json(Game_state *state, cJSON *json);

// Initialize game at the beggiing of the game
void initialize_Game(Game_state *currentState);

#endif