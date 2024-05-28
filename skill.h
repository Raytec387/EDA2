// Created 15/05/2024

#ifndef SKILL_H
#define SKILL_H

#include "common.h"

#define MAX_SKILL 4
#define MAX_SKILL_IN_GAME 8
#define MAX_STACK_SIZE 20

// stack structure for Time_Strike
typedef struct {
    int stack[MAX_STACK_SIZE];
    int top;
} ability_stack;

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
// Dictionary to record the number of used skill
typedef struct AbilityTracker {
    Ability *table[MAX_SKILL_IN_GAME];
} AbilityTracker;

// Function definitions

void load_skill(const char *filename, Skill *skills);

void change_skill(const char *filename, Skill *skills);

// Dictionary part
unsigned int hash(const char *str);                                     // Avoid overflow
AbilityTracker* create_tracker();                                       // Create a tracker
void use_ability(AbilityTracker *tracker, const char *name);            // Add ability to the dicctionary
void display_abilities(const AbilityTracker *tracker);                  // Show used skill, and times used in the terminal
void free_tracker(AbilityTracker *tracker);                             // Free the tracker
void save_tracker(const AbilityTracker *tracker, const char *filename); // Save to ability.txt, if file is missing, create one
AbilityTracker* load_tracker(const char *filename);                     // Load from ability.txt

#endif