#include <stdio.h>
#include "common.h"

int main(){
    cJSON *json = create_json("stats.json");
    const cJSON *Main_character = NULL;
    const cJSON *main_character = NULL;
    // access the JSON data 

    Main_character = cJSON_GetObjectItemCaseSensitive(json, "Main_Character");

    cJSON *atk = cJSON_GetObjectItemCaseSensitive(Main_character, "ATK");
    cJSON *def = cJSON_GetObjectItemCaseSensitive(Main_character, "DEF");
    // return code
    cJSON_Delete(json);

    printf("Attack: %f\n", atk->valuedouble);
    printf("Defence: %d\n", def->valueint);
}