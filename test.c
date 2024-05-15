#include <stdio.h>
#include "common.h"
#include "common.c"

int main(){
    cJSON *json = create_json("stats.json");
    const cJSON *Main_character = NULL;
    const cJSON *main_character = NULL;
    int *atk = NULL;
    // access the JSON data 

    Main_character = cJSON_GetObjectItemCaseSensitive(json, "Main_Character");
    cJSON_ArrayForEach(main_character, Main_character)
    {
        cJSON *atk = cJSON_GetObjectItemCaseSensitive(main_character, "ATK");
        cJSON *def = cJSON_GetObjectItemCaseSensitive(main_character, "DEF");
    }
    // return code
    cJSON_Delete(json);
}