// Created 15/05/2024

#ifndef SKILL_H
#define SKILL_H

#include "common.h"

#define MAX_SKILL 4
#define MAX_SKILL_IN_GAME 8

// Forward declaration of Character struct
typedef struct Character Character;

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

// Struct for condtions, skills that have certain condition
// E.g. activation the skill will decrease hp by 50%
// E.g. activating skill will apply a def debuff on user
// Reuse some of skill functions
typedef struct {
    Skill_type type;
    Target target;
    float value;
    int is_percentile;
    Effect effect;
} Condition;

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
    Condition condition;

    // adding int to track the skill
    int id;
} Skill;

// Function defnitions
int damage(float damage, int def);

void heal(float value, Character *character);

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