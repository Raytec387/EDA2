#include "main.h"
#include "scenario.c"

void run(){
    int option;
    do{
        // Track the scenario
        Game_state currentState;
        currentState.currentScenarioId = 1;

        printf("\033[2J\033[1;1H");
        printf("Welcome to Red rain,\n");
        printf("Select an option (%d-%d)to start:\n", EXIT_BACK, SAVE_GAME);
        displayMenu();
        option = check_input(EXIT_BACK, SAVE_GAME);
        switch(option){
            case START_GAME:
                start_game(currentState);
                break;
            case LOAD_GAME:
                load_game(currentState);
                break;
            case SAVE_GAME:
                save_game(currentState);
                break;
        }
    } while (option != EXIT_BACK);
}

void start_game(Game_state currentState) {
    /// Initialize the game ///
    // Create scenarios
    Scenario* scenarios[MAX_SCENARIO];
    graph_initialize(scenarios);

    // Start with the first scenario
    Scenario* currentScenario = scenarios[currentState.currentScenarioId-1];
    story_Navi_battleCheck(scenarios,currentState, currentScenario);
    // Free allocated memory
    freeScenarios(scenarios, MAX_SCENARIO);
    return;
}

void load_game(Game_state currentState){
    // TO DO
    return;
}

void save_game(Game_state currentState){
    // TO DO
    return;
}

// Game state functions ------------------------------------------------------

Game_state* make_game_state() {
    Game_state *new_game_state = malloc(sizeof(*new_game_state));
    return new_game_state;
}

// Makes a copy of the game state
Game_state* copy_game_state(Game_state *game_state) {
    Game_state *new_game_state = make_game_state();
    new_game_state->character = game_state->character; // Shallow copy, change if character uses pointers
    return new_game_state;
}

// Frees game state is it isn't null
void free_game_state(Game_state *game_state) {
    if (game_state != NULL) {
        free(game_state);
    }
}

// Session functions ---------------------------------------------------------

// Initialize session struct
void init_session(Session *session) {
    session->current_game_state = NULL;
    session->prev_game_state = NULL;
}

// Remove content in session and make a new one
void restart_session(Session *session) {
    // TO DO: Game state functions
}

int main(){
    run();
}
