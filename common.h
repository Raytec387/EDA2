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
//#include "cJSON.c"


#define SIZE_OF_BUFFER 2048
#define MAX_STR_NAME 20
#define MAX_SCENARIO 5

// Character Macros
#define NAME_LENGTH 20
#define DESCRITPION_LENGTH 100
#define MAX_SKILL 4
#define MAX_SKILL_IN_GAME 8

// all skill in the game THIS BREAKS THE CODE! BAD IDEA
/*char All_skill[MAX_SKILL_IN_GAME][NAME_LENGTH] = {  {"Defence"},{"Strong Hit"},{"Heal"},{"Concentrate"},
                                                    {"Heal+"},{"Good Swordart"},{"Wind Field"},{"Last Stardust"}};
*/

// Menu Options
#define EXIT_BACK 0
#define START_GAME 1
#define LOAD_GAME 2
#define SAVE_GAME 3

cJSON *create_json(const char *file_name);

int check_input(int lower,int upper);

#endif