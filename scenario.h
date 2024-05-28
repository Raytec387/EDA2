#ifndef SCENARIO_H
#define SCENARIO_H

#include "common.h"
#include "game.h"
#include "combat.h"

// Structure to represent a scenario (node)
typedef struct Scenario {
    int id;
    char name[NAME_LENGTH];
    bool battleWon;
    struct Scenario* adjacent[MAX_SCENARIO];
    int adjCount;
} Scenario;

#endif