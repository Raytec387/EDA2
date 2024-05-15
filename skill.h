// Created 15/05/2024

#ifndef _SKILL_H_
#define _SKILL_H_

#include "common.h"
#include "character.h"

typedef enum {
    SELF,
    TARGET,
    CROWD_TARGET,
    CROWD_SELF
} Skill_type;

typedef enum {
    DMG_OVER_TIME,
    HEAL_OVER_TIME,
    DEF_BUFF,
    HP_BUFF,
    ATK_BUFF
} Effect_type;

typedef struct {
    Effect_type type;
    float value;
    int duration;
} Effect;

typedef struct {
    char name[NAME_LENGTH];
    char desc[DESCRITPION_LENGTH];
    Skill_type type;
    float value;
    int cooldown;
    int remaining_cooldown;
    Effect effect;
} Skill;

#endif