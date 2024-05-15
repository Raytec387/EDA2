#include <stdio.h>
#include "common.h"

int main(){
    cJSON *json = create_json("stats.json");

    // access the JSON data 
    cJSON *value = cJSON_GetObjectItemCaseSensitive(json, "enemies" "bullier" "ATK"); 
    printf("%d\n", value->valueint);

    // return code
    cJSON_Delete(json);
}