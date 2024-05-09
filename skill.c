#include "common.h"

int main() { 
    // open the file 
    FILE *fp = fopen("skill.json", "r"); 
    if (fp == NULL) { 
        printf("Error: Unable to open the file.\n"); 
        return 1; 
    } 
  
    // read the file contents into a string 
    char buffer[SIZE_OF_BUFFER]; 
    int len = fread(buffer, 1, sizeof(buffer), fp); 
    fclose(fp); 
  
    // parse the JSON data 
    cJSON *json = cJSON_Parse(buffer); 
    if (json == NULL) { 
        const char *error_ptr = cJSON_GetErrorPtr(); 
        if (error_ptr != NULL) { 
            printf("Error: %s\n", error_ptr); 
        } 
        cJSON_Delete(json); 
        return 1; 
    } 
  
    // access the JSON data 
    cJSON *description = cJSON_GetObjectItemCaseSensitive(json, "Defence"); 
    if (cJSON_IsString(description) && (description->valuestring != NULL)) { 
        printf("Name: %s\n", description->valuestring); 
        char s[100];
        strcpy(s,description->valuestring);
        cJSON *skill= cJSON_GetObjectItemCaseSensitive(json, s); 
        printf("Name: %d\n", skill->valueint); 
    } 
  
    // delete the JSON object 
    cJSON_Delete(json); 
    return 0; 
}