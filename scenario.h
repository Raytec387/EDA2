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

// Graph part, done with adjacenncy list
Scenario* createScenario(int id, const char* name);                     // Initialize the nodes in the graph
void addEdge(Scenario* src, Scenario* dest);                            // Add edge
void showAdjacentScenarios(Scenario* scenario);                         // Show available adjacent scenario
void graph_initialize(Scenario *scenarios[MAX_SCENARIO]);               // Build edges for each node in the graph
Scenario* getScenarioById(Scenario* scenarios[], int id, int count);    // Function to get a scenario by its ID
void freeScenarios(Scenario* scenarios[], int count);                   // Free the momory

// Execute the story of the game from txt file
int scenario_txt(char txt[NAME_LENGTH]);                                // The story of the game, return number of enemies
void scenario_end_txt(char txt[NAME_LENGTH]);                           // The story of the game after the combat
// Call the scenario_txt() and scenario_end_txt() functions, and check if user won/lost combat
void story_Navi_battleCheck(Scenario *scenarios[MAX_SCENARIO], Game_state *currentState, Scenario* currentScenario);

#endif