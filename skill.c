#include "skill.h"

int time_strike_use = 0;
int top = -1;

// Load Skill from Json
void load_skill(const char *filename, Skill *skills) {

    // Open json file
    cJSON *json = create_json(filename);
    int i;
    for (int j = 0; j < MAX_SKILL; j++) {
        i = skills[j].id;
        // Read skills array
        cJSON *skill_json = cJSON_GetArrayItem(json, i);

        // Read skills
        strncpy(skills[i].name, cJSON_GetObjectItem(skill_json, "name")->valuestring, NAME_LENGTH);
        strncpy(skills[i].desc, cJSON_GetObjectItem(skill_json, "description")->valuestring, DESCRITPION_LENGTH);
        skills[i].type = cJSON_GetObjectItem(skill_json, "type")->valueint;
        skills[i].target = cJSON_GetObjectItem(skill_json, "target")->valueint;
        skills[i].value = cJSON_GetObjectItem(skill_json, "value")->valuedouble;
        skills[i].is_percentile = cJSON_GetObjectItem(skill_json, "is_percentile")->valueint;
        skills[i].cooldown = cJSON_GetObjectItem(skill_json, "cooldown")->valueint;

        // Read effect
        cJSON *effect_json = cJSON_GetObjectItem(skill_json, "effects");
        skills[i].effect.type = cJSON_GetObjectItem(effect_json, "type")->valueint;
        skills[i].effect.value = cJSON_GetObjectItem(effect_json, "value")->valuedouble;
        skills[i].effect.duration = cJSON_GetObjectItem(effect_json, "duration")->valueint;
    }
    cJSON_Delete(json);
}

void change_skill(const char *filename, Skill *skills) {

    // Open json file
    cJSON *json = create_json(filename);
    Skill temp_skill;
    printf("\nAll skills and descriptions\n");
    // Show the option
    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        // Read skills array
        cJSON *skill_json = cJSON_GetArrayItem(json, i);

        // Read skills
        strncpy(temp_skill.name, cJSON_GetObjectItem(skill_json, "name")->valuestring, NAME_LENGTH);
        strncpy(temp_skill.desc, cJSON_GetObjectItem(skill_json, "description")->valuestring, DESCRITPION_LENGTH);
        printf("ID: %d\t%s:\n%s\n",i+1,temp_skill.name,temp_skill.desc);
    }
    printf("\nChoose a skill you want to change\n(type 0 to exit):\n");
    int wanted_skill = check_input(0,MAX_SKILL_IN_GAME);
    if (wanted_skill == 0) return;
    // Id in skill.json start with 0, output is 1
    wanted_skill -=1;
    printf("\nChoose a skill that you want to substitute:\n");
    for (int i=0; i< MAX_SKILL;i++){
        printf("ID: %d\t%s:\n%s\n",i+1,skills[i].name,skills[i].desc);
    }
    int chosen_skill = check_input(1,MAX_SKILL);
    chosen_skill -=1;

    skills[chosen_skill].id = wanted_skill;
    cJSON_Delete(json);
}
///////////                                                                                 ///////////
/////////// Dictionary for counting the number of the skill that main character used.       ///////////
///////////                                                                                 ///////////

// Hash function for strings
unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % MAX_SKILL_IN_GAME;
}

// Create a new tracker
AbilityTracker* create_tracker() {
    AbilityTracker *tracker = (AbilityTracker *)malloc(sizeof(AbilityTracker));
    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        tracker->table[i] = NULL;
    }
    return tracker;
}

// Use an ability
void use_ability(AbilityTracker *tracker, const char *name) {
    unsigned int index = hash(name);
    Ability *current = tracker->table[index];

    // Check if the ability already exists
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->count++;
            return;
        }
        current = current->next;
    }

    // Ability does not exist, create a new entry
    Ability *new_ability = (Ability *)malloc(sizeof(Ability));
    new_ability->name = strdup(name);
    new_ability->count = 1;
    new_ability->next = tracker->table[index];
    tracker->table[index] = new_ability;
}

// Display all abilities
void display_abilities(const AbilityTracker *tracker) {
    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        Ability *current = tracker->table[i];
        while (current != NULL) {
            printf("Ability: %s, Count: %d\n", current->name, current->count);
            current = current->next;
        }
    }
}

// Free the tracker
void free_tracker(AbilityTracker *tracker) {
    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        Ability *current = tracker->table[i];
        while (current != NULL) {
            Ability *to_free = current;
            current = current->next;
            free(to_free->name);
            free(to_free);
        }
    }
    free(tracker);
}

// Save the tracker to a file
void save_tracker(const AbilityTracker *tracker, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < MAX_SKILL_IN_GAME; i++) {
        Ability *current = tracker->table[i];
        while (current != NULL) {
            fprintf(file, "%s %d\n", current->name, current->count);
            current = current->next;
        }
    }

    fclose(file);
}

// Load the tracker from a file
AbilityTracker* load_tracker(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    AbilityTracker *tracker = create_tracker();
    char name[NAME_LENGTH];
    int count;

    while (fscanf(file, "%s %d", name, &count) == 2) {
        
        unsigned int index = hash(name);
        Ability *new_ability = (Ability *)malloc(sizeof(Ability));
        new_ability->name = strdup(name);
        new_ability->count = count;
        new_ability->next = tracker->table[index];
        tracker->table[index] = new_ability;
    }

    fclose(file);
    return tracker;

}


/* Example to use the dictionary
int main() {
    AbilityTracker *tracker = create_tracker();

    use_ability(tracker, "Fireball");
    use_ability(tracker, "Ice Blast");
    use_ability(tracker, "Fireball");

    display_abilities(tracker);

    free_tracker(tracker);
    return 0;
}
*/

///////////                                                                                 ///////////
/////////// Dictionary for counting the number of the skill that main character used.       ///////////
///////////                                                                                 ///////////