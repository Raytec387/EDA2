#include "main.h"                                                                              ///////////

void run(Game_state *currentState) {
    int option;
    // Track the scenario
    printf("\033[2J\033[1;1H");
    printf("%d\n", currentState->currentScenarioId);
    printf("Welcome to Red rain,");
    do{
        printf("\n\n");
        printf("Select an option (%d-%d)to start:\n", EXIT_BACK, SHOW_SKILL_TIMES);
        displayMenu();
        option = check_input(EXIT_BACK, SHOW_SKILL_TIMES);
        switch(option){
            case START_GAME:
                start_game(currentState);
                break;
            case LOAD_GAME:
                load_game(SAVE_FILE, currentState);
                break;
            case SAVE_GAME:
                save_game(SAVE_FILE,currentState);
                break;
            case SKILL_CHANGE:
                change_skill(SKILL_FILE,currentState->character.skill_array);
                load_skill(SKILL_FILE,currentState->character.skill_array);
                break;
            case SHOW_SKILL_TIMES:
                display_abilities(currentState->tracker);
                break;
                
        }
    } while (option != EXIT_BACK);
}

void start_game(Game_state *currentState) {
    /// Initialize the game ///
    // Create scenarios
    Scenario* scenarios[MAX_SCENARIO];
    graph_initialize(scenarios);
    
    // Start with the first scenario
    Scenario* currentScenario = scenarios[currentState->currentScenarioId-1];
    story_Navi_battleCheck(scenarios,currentState, currentScenario);
    // Free allocated memory
    freeScenarios(scenarios, MAX_SCENARIO);
    return;
}

void load_game(const char *file_name, Game_state *currentState){
    currentState->tracker = load_tracker(TRACKER_FILE);
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error: Unable to open file for reading.\n");
        return;
    }

    char buffer[SIZE_OF_BUFFER];
    fread(buffer, 1, SIZE_OF_BUFFER, fp);
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return;
    }

    parse_game_state_from_json(currentState, json);
    cJSON_Delete(json);
    load_skill(SKILL_FILE,currentState->character.skill_array);
    currentState->character.is_player = true;
    strcpy(currentState->character.name,"David");
    start_game(currentState);
    return;
}

void save_game(const char *file_name, Game_state *currentState){
    save_tracker(currentState->tracker,TRACKER_FILE);
    cJSON *json = create_json_from_game_state(currentState);
    char *json_string = cJSON_Print(json);

    FILE *fp = fopen(file_name, "w");
    if (fp == NULL) {
        printf("Error: Unable to open file for writing.\n");
        cJSON_Delete(json);
        free(json_string);
        return;
    }

    fprintf(fp, "%s", json_string);
    fclose(fp);

    cJSON_Delete(json);
    free(json_string);
    return;
}

// Session functions ---------------------------------------------------------

/* Not necessary
// Initialize session struct
void init_session(Session *session) {
    session->current_game_state = NULL;
    session->prev_game_state = NULL;
}

// Remove content in session and make a new one
void restart_session(Session *session) {
    // TO DO: Game state functions
}
*/

int main(){
    Game_state *game = make_game_state();
    initialize_Game(game);
    run(game);
}
