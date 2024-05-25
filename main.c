#include "main.h"
#include "scenario.c"

///////////                                                                                 ///////////
/////////// Dictionary for counting the number of the skill that main character used.       ///////////
///////////                                                                                 ///////////


// Hash function for strings
unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % MAX_SKILL_IN_GAME;
}

// Create a new tracker
AbilityTracker* create_tracker() {
    AbilityTracker *tracker = (AbilityTracker *)malloc(sizeof(AbilityTracker));
    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        tracker->table[i] = NULL;
    }
    return tracker;
}

// Use an ability
void use_ability(AbilityTracker *tracker, const char *name) {
    unsigned int index = hash(name);
    Ability *current = tracker->table[index];

    // Check if the ability already exists
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->count++;
            return;
        }
        current = current->next;
    }

    // Ability does not exist, create a new entry
    Ability *new_ability = (Ability *)malloc(sizeof(Ability));
    new_ability->name = strdup(name);
    new_ability->count = 1;
    new_ability->next = tracker->table[index];
    tracker->table[index] = new_ability;
}

// Display all abilities
void display_abilities(const AbilityTracker *tracker) {
    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        Ability *current = tracker->table[i];
        while (current != NULL) {
            printf("Ability: %s, Count: %d\n", current->name, current->count);
            current = current->next;
        }
    }
}

// Free the tracker
void free_tracker(AbilityTracker *tracker) {
    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        Ability *current = tracker->table[i];
        while (current != NULL) {
            Ability *to_free = current;
            current = current->next;
            free(to_free->name);
            free(to_free);
        }
    }
    free(tracker);
}

// Save the tracker to a file
void save_tracker(const AbilityTracker *tracker, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        Ability *current = tracker->table[i];
        while (current != NULL) {
            fprintf(file, "%s %d\n", current->name, current->count);
            current = current->next;
        }
    }

    fclose(file);
}

// Load the tracker from a file
AbilityTracker* load_tracker(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    AbilityTracker *tracker = create_tracker();
    char name[NAME_LENGTH];
    int count;

    while (fscanf(file, "%s %d", name, &count) == 2) {
        
        unsigned int index = hash(name);
        Ability *new_ability = (Ability *)malloc(sizeof(Ability));
        new_ability->name = strdup(name);
        new_ability->count = count;
        new_ability->next = tracker->table[index];
        tracker->table[index] = new_ability;
    }

    fclose(file);
    return tracker;

}
///////////                                                                                 ///////////
/////////// Dictionary for counting the number of the skill that main character used.       ///////////
///////////                                                                                 ///////////

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

/// @brief save and load ///
// Create a JSON object from the Game_state
cJSON *create_json_from_game_state(Game_state *state) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "currentScenarioId", state->currentScenarioId);

    cJSON *character = cJSON_CreateObject();
    cJSON_AddNumberToObject(character, "ATK", state->character.atk);
    cJSON_AddNumberToObject(character, "DEF", state->character.def);
    cJSON_AddNumberToObject(character, "HP", state->character.hp_limit);
    cJSON_AddNumberToObject(character, "SKILL_ID1", state->character.skill_array[0].id);
    cJSON_AddNumberToObject(character, "SKILL_ID2", state->character.skill_array[1].id);
    cJSON_AddNumberToObject(character, "SKILL_ID3", state->character.skill_array[2].id);
    cJSON_AddNumberToObject(character, "SKILL_ID4", state->character.skill_array[3].id);
    cJSON_AddItemToObject(json, "character", character);

    return json;
}
// Parse Game_state from a JSON object
void parse_game_state_from_json(Game_state *state, cJSON *json) {
    state->currentScenarioId = cJSON_GetObjectItemCaseSensitive(json, "currentScenarioId")->valueint;

    cJSON *character = cJSON_GetObjectItemCaseSensitive(json, "character");
    state->character.atk = cJSON_GetObjectItemCaseSensitive(character, "ATK")->valueint;
    state->character.def = cJSON_GetObjectItemCaseSensitive(character, "DEF")->valueint;
    state->character.hp = (float)cJSON_GetObjectItemCaseSensitive(character, "HP")->valuedouble;
    state->character.skill_array[0].id = cJSON_GetObjectItemCaseSensitive(character, "SKILL_ID1")->valueint;
    state->character.skill_array[1].id = cJSON_GetObjectItemCaseSensitive(character, "SKILL_ID2")->valueint;
    state->character.skill_array[2].id = cJSON_GetObjectItemCaseSensitive(character, "SKILL_ID3")->valueint;
    state->character.skill_array[3].id = cJSON_GetObjectItemCaseSensitive(character, "SKILL_ID4")->valueint;
}

/// @brief save and load ///

Game_state *initialize_Game(){
    Game_state *currentState = make_game_state();
    currentState->currentScenarioId = 1;
    currentState->tracker = create_tracker();
    // Initialize character part
    currentState->character.atk = 150;
    currentState->character.def = 300;
    currentState->character.hp_limit = 1000.0;
    currentState->character.skill_array[0].id = 0;
    currentState->character.skill_array[1].id = 1;
    currentState->character.skill_array[2].id = 2;
    currentState->character.skill_array[3].id = 3;
    return currentState;
}

void run(Game_state *currentState){
    int option;
    // Track the scenario

    printf("\033[2J\033[1;1H");
    printf("%d\n",currentState->currentScenarioId);
    printf("Welcome to Red rain,");
    do{
        printf("\n\n");
        printf("Select an option (%d-%d)to start:\n", EXIT_BACK, SHOW_SKILL_TIMES);
        displayMenu();
        option = check_input(EXIT_BACK, SHOW_SKILL_TIMES);
        switch(option){
            case START_GAME:
                start_game(initialize_Game());
                break;
            case LOAD_GAME:
                load_game(SAVE_FILE,currentState);
                break;
            case SAVE_GAME:
                save_game(SAVE_FILE,currentState);
                break;
            case SKILL_CHANGE:
                // A change skill function to do
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
    start_game(currentState);
    return;
}

void save_game(const char *file_name,Game_state *currentState){
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
    run(initialize_Game());
}
