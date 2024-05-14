// Created on 24/04/2024
// common.h is for storing the useful head file

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON.c"

<<<<<<< HEAD

#define SIZE_OF_BUFFER 2048
#define MAX_STR_NAME 20
#define MAX_SCENARIO 5
=======
// Character Macros
>>>>>>> 1259ee2 (Changes to main, added new file to have all the "UI" element functions)
#define NAME_LENGTH 20
#define DESCRITPION_LENGTH 100
#define MAX_SKILL 4
#define MAX_SKILL_IN_GAME 8

// all skill in the game
char All_skill[MAX_SKILL_IN_GAME][NAME_LENGTH] = {  {"Defence"},{"Strong Hit"},{"Heal"},{"Concentrate"},
                                                    {"Heal+"},{"Good Swordart"},{"Wind Field"},{"Last Stardust"}};

// Menu Options
#define EXIT_BACK 0
#define START_GAME 1
#define LOAD_GAME 2
#define SAVE_GAME 3

#endif