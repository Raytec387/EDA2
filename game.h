#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "character.h"
#include "skill.h"

typedef struct {
    int top;
    int stack[MAX_STACK_SIZE];
    int time_strike_use;
} Time_strike_stack;

typedef struct {
    // keep track of current scenario
    int currentScenarioId;
    Character character; // Keeps track of the current character
    AbilityTracker *tracker;
    Time_strike_stack ability_stack;
} Game_state;

Game_state* make_game_state();

Game_state* copy_game_state(Game_state *game_state);

void free_game_state(Game_state *game_state);

cJSON *create_json_from_game_state(Game_state *state);

void parse_game_state_from_json(Game_state *state, cJSON *json);

void initialize_Game(Game_state *currentState);

#endif