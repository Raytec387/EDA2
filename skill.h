// Created 15/05/2024

#ifndef SKILL_H
#define SKILL_H

#include "common.h"

#define MAX_SKILL 4
#define MAX_SKILL_IN_GAME 8
#define MAX_STACK_SIZE 20
#define SKILL_FILE "skill.json"
int time_strike_use = 0;
int stack[MAX_STACK_SIZE];
int top = -1;

// Type of skill, target self, target others, single or group...
typedef enum {
    HEAL,
    DAMAGE,
    BUFF,
    DEBUFF
} Skill_type;

typedef enum {
    SELF,
    TARGET,
    CROWD_SELF,
    CROWD_TARGET
} Target;

// Type of effect
typedef enum {
    DMG_OVER_TIME,
    HEAL_OVER_TIME,
    DEF,
    HP,
    ATK
} Effect_type;

// Struct containing effect data
// If it's a debuff, put value to 0 < x < 1 if percentile or negative if flat
typedef struct {
    Effect_type type;
    float value;
    int is_percentile;
    int duration;
} Effect;

// Main struct for skills
typedef struct {
    char name[NAME_LENGTH];
    char desc[DESCRITPION_LENGTH];
    Skill_type type;
    Target target;
    float value;
    int is_percentile;
    int cooldown;
    int remaining_cooldown;
    Effect effect;

    // adding int to track the skill
    int id;
} Skill;

// Ability tracer
typedef struct Ability {
    char *name;
    int count;
    struct Ability *next;
} Ability;

typedef struct AbilityTracker {
    Ability *table[MAX_SKILL_IN_GAME];
} AbilityTracker;

#endif