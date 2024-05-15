// Created 15/05/2024

#ifndef SKILL_H
#define SKILL_H

#include "common.h"

// Forward declaration of Character struct
struct Character;

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