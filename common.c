// Created on 10/5/2024
// This file stores the useful function that will call 
// Many times when building the project

#include "common.h"

// Create a json pointer to access to the json file
cJSON *create_json(const char *file_name){
    // open the file 
    FILE *fp = fopen(file_name, "r"); 
    if (fp == NULL) { 
        printf("Error: Unable to open the file.\n"); 
    } 
  
    // read the file contents into a string 
    char buffer[SIZE_OF_BUFFER]; 
    fread(buffer, 1, sizeof(buffer), fp); 
    fclose(fp); 
  
    // parse the JSON data 
    cJSON *json = cJSON_Parse(buffer); 
    if (json == NULL) { 
        const char *error_ptr = cJSON_GetErrorPtr(); 
        if (error_ptr != NULL) { 
            printf("Error: %s\n", error_ptr); 
        } 
        cJSON_Delete(json); 
    }

    return json;
}

/*
// Create a json pointer to access to the json file
cJSON *create_json(char file_name[NAME_LENGTH]) {
    FILE *file = fopen(file_name, "r");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *json_buffer = malloc(file_size + 1);

    fread(json_buffer, 1, file_size, file);
    fclose(file);
    json_buffer[file_size] = '\0';

    cJSON *root = cJSON_Parse(json_buffer);

    return root;
}
*/

// This function checks if the input is in the interval
int check_input(int lower,int upper){
    int option;
    if(scanf("%d",&option) != 1)  {printf("Error!"); return -1;} 
    while(!(option >= lower && option <= upper)){
        printf("Please enter a valid option (%d-%d): ", lower, upper);
        scanf("%d",&option);
    }
    printf("\n");
    return option;
}