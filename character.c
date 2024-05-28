// Created 14/05/2024

#include "character.h"

// All function related to character creation is in here

Character* create_new_enemy(){
    Character *new_enemy = malloc(sizeof(*new_enemy));
    return new_enemy;
}

void initialize_enemy(int i, Character *enemy){
    // Open json file
    cJSON *json = create_json("stats.json");
    // First scenario is 1, and in the stats.json, the first enemy's id is 0,therefore -1
    i--;
    // Read stats(enemy) array
    cJSON *character_json = cJSON_GetArrayItem(json, i);

    // Read an enemy
    strncpy(enemy->name, cJSON_GetObjectItem(character_json, "NAME")->valuestring, NAME_LENGTH);
    enemy->atk = cJSON_GetObjectItem(character_json, "ATK")->valueint;
    enemy->def = cJSON_GetObjectItem(character_json, "DEF")->valueint;
    enemy->hp_limit = cJSON_GetObjectItem(character_json, "HP")->valuedouble;
    enemy->skill_array[0].id = cJSON_GetObjectItem(character_json, "SKILL_ID1")->valueint;
    enemy->skill_array[1].id = cJSON_GetObjectItem(character_json, "SKILL_ID2")->valueint;
    enemy->skill_array[2].id = cJSON_GetObjectItem(character_json, "SKILL_ID3")->valueint;
    enemy->skill_array[3].id = cJSON_GetObjectItem(character_json, "SKILL_ID4")->valueint;
    load_skill(SKILL_FILE,enemy->skill_array);
    enemy->is_player = false;

    cJSON_Delete(json);
}

void initialize_enemies_array(int id_enemy, Character *enemy[], int number_of_enemies){
    for(int i = 0; i<number_of_enemies;i++){
        enemy[i] = create_new_enemy();
        initialize_enemy(id_enemy,enemy[i]);
    }
}

void initialize_main_character(Character *player){
    // Initialize character part
    player->atk = 150;
    player->def = 300;
    player->hp_limit = 1000.0;
    player->skill_array[0].id = 0;
    player->skill_array[1].id = 1;
    player->skill_array[2].id = 2;
    player->skill_array[3].id = 3; 
    load_skill(SKILL_FILE,player->skill_array);
    player->is_player = true;
    strcpy(player->name,"David");
    player->ability_stack.top = -1;
    player->ability_stack.time_strike_use = 0;
}

