#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "character.h"
#include "skill.h"

typedef struct {
    // keep track of current scenario
    int currentScenarioId;
    Character character; // Keeps track of the current character
    AbilityTracker *tracker;
} Game_state;

#endif