#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include "character.h"

typedef struct {
    // keep track of current scenario
    int currentScenarioId;
    Character character; // Keeps track of the current character
} Game_state;

typedef struct {
    // Info about current session
    // Keeps track of the game state e.g. character info and scenario info
    // prev_game_state is a checkpoint, where user can revert back to
    // WIP
    Game_state *current_game_state;
    Game_state *prev_game_state;
} Session;

// Structure to represent a scenario (node)
typedef struct Scenario {
    int id;
    char name[NAME_LENGTH];
    bool battleWon;
    struct Scenario* adjacent[MAX_SCENARIO];
    int adjCount;
} Scenario;

void run();
void start_game();
void load_game();
void save_game();

#endif