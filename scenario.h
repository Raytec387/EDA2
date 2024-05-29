#ifndef SCENARIO_H
#define SCENARIO_H

#include "common.h"
#include "game.h"
#include "combat.h"

// Structure to represent a scenario (The node in the graph)
typedef struct Scenario {
    int id;
    char name[NAME_LENGTH];
    bool battleWon;
    struct Scenario* adjacent[MAX_SCENARIO];
    int adjCount;
} Scenario;

Scenario* createScenario(int id, const char* name); 
void addEdge(Scenario* src, Scenario* dest); 
void showAdjacentScenarios(Scenario* scenario);
 
Scenario* getScenarioById(Scenario* scenarios[], int id, int count); 
void freeScenarios(Scenario* scenarios[], int count); 
int scenario_txt(char txt[NAME_LENGTH]);
void scenario_end_txt(char txt[NAME_LENGTH]);
void graph_initialize(Scenario *scenarios[MAX_SCENARIO]);
void story_Navi_battleCheck(Scenario *scenarios[MAX_SCENARIO], Game_state *currentState, Scenario* currentScenario);

#endif