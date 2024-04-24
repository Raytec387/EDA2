// Created by zihao zhou on 2024/4/24.
// This head will hold the data structure of the role(enemy or main character)

#define MAX_NAME 20
#define MAX_SKILL 4

typedef struct {
    char name[MAX_NAME];
    int atk;
    int hp;
    int def;
    int skill_array[MAX_SKILL];
}Main_character;

typedef struct {
    char name[MAX_NAME];
    int atk;
    int hp;
    int def;
}Enemy;

// Main struct handling skills functionality
typedef struct Skills {
    char name[20];
    int attack;
    int type;
    int uses;
    int cooldown;
    char desc[200];
} Skills;

#ifndef CHARACTER_H
#define CHARACTER_H

#endif //CHARACTER_H

