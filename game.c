#include "game.h"

// Game state functions ------------------------------------------------------

Game_state* make_game_state() {
    Game_state* new_game_state = malloc(sizeof(*new_game_state));
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

void initialize_Game(Game_state *currentState) {
    currentState->currentScenarioId = 1;
    currentState->tracker = create_tracker();
    // function from character.h
    initialize_main_character(currentState);
}