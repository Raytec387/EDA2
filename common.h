// Created on 24/04/2024
// common.h is for storing the useful head file

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "cJSON.h"


#define SIZE_OF_BUFFER 8192
#define MAX_SCENARIO 5

// Character Macros
#define NAME_LENGTH 20
#define DESCRITPION_LENGTH 100
#define MAX_ACTIVE_EFFECTS 8

// File Macros
#define TRACKER_FILE "ability.txt"
#define SAVE_FILE "save.json"
#define SKILL_FILE "skill.json"

// Menu Options
#define EXIT_BACK 0
#define START_GAME 1
#define LOAD_GAME 2
#define SAVE_GAME 3
#define SKILL_CHANGE 4
#define SHOW_SKILL_TIMES 5

// Read json file, create and return a address 
cJSON *create_json(const char *file_name);

// Check if the input is in the range(lower - upper), both end included
int check_input(int lower,int upper);

#endif