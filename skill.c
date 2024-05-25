#include "skill.h"
#include "character.h"
#include "common.c"
#include "main.h"

// Damage calculation - damage vs defense
int damage(float damage, int def){
    return (damage * 100/(100+def));
}

// Healing function
void heal(float value, Character *character) {
    if (character->hp + value > character->hp_limit) {
        character->hp = character->hp_limit;
    } else {
        character->hp += value;character->hp = character->hp_limit;
    }
}

void apply_active_effect(Character *character){
    for (int i = 0; i < MAX_ACTIVE_EFFECTS; i++){
        Effect current_effect = character->active_effects[i];
        if (current_effect.duration > 0){
            switch (current_effect.type)
            {
            case DMG_OVER_TIME:
                character->hp -= damage(current_effect.value, character->def);
                printf("%s suffers %d of damage from ongoing effects\n", character->name, damage(current_effect.value, character->def));
                break;
            case HEAL_OVER_TIME:
                character->hp += current_effect.value;
                printf("%s heals for %f from ongoing effects\n", character->name, current_effect.value);
            case DEF:
                /* Solution: Apply buff at skill usage*/
                /* Remove buff here once its duration is 0*/
                if (current_effect.duration <= 0) {
                    if (current_effect.is_percentile) {
                        character->def /= current_effect.value;
                    } else {
                        character->def -= current_effect.value;
                    }
                    printf("Effect affecting %s's defence has worn out!\n", character->name);
                }
                break;
            case HP:
                if (current_effect.duration <= 0) {
                    if (current_effect.is_percentile) {
                        character->hp_limit /= current_effect.value;
                    } else {
                        character->hp_limit -= current_effect.value;
                    }
                    printf("Effect affecting %s's max hp has worn out!\n", character->name);
                }
            case ATK:
                if (current_effect.duration <= 0) {
                    if (current_effect.is_percentile) {
                        character->atk /= current_effect.value;
                    } else {
                        character->atk -= current_effect.value;
                    }
                    printf("Effect affecting %s's attack has worn out!\n", character->name);
                }
            default:
                break;
            }
        }
    }
}


// Funtion to apply the actual skill
void apply_skill(Skill *skill, Character *user, Character *target, Game_state* currentState) {
    float heal_amount;
    float damage_amount;
    // For dictionary and Timestrike(with stack)
    if(user->is_player) {
        use_ability(currentState->tracker, skill->name);   
        push(skill->id);
    }
    switch (skill->type) {
        case HEAL:
            if (skill->is_percentile) {
                heal_amount = skill->value * (target->hp_limit - target->hp); // n% of missing
            } else {
                heal_amount = skill->value;
            }
            heal(heal_amount, target);
            printf("%s healed for %f hp!. \n", target->name, heal_amount);
            break;
        case DAMAGE:
            if (skill->is_percentile) {
                damage_amount = skill->value * user->atk;
            } else {
                damage_amount = skill->value;
            }
            damage_amount = damage(damage_amount, target->def);

            printf("%s dealt %f damage to %s", user->name, damage_amount, target->name);
            break;
        case BUFF:
        case DEBUFF:
            switch (skill->effect.type) {
                float buff_amount;
                case DEF:
                    if (skill->effect.is_percentile) {
                        buff_amount = (skill->effect.value * target->def) - target->def;
                    } else {
                        buff_amount = skill->effect.value;
                    }
                    target->def += buff_amount;
                    if (skill->type == BUFF) {
                        printf("%s defence increased by %f!\n", target->name, buff_amount);
                    } else {
                        printf("%s defence decreased by %f!\n", target->name, buff_amount);
                    }
                    break;
                case HP:
                    if (skill->effect.is_percentile) {
                        buff_amount = (skill->effect.value * target->hp_limit) - target->hp_limit;
                    } else {
                        buff_amount = skill->effect.value;
                    }
                    target->hp_limit += buff_amount;
                    if (skill->type == BUFF) {
                        printf("%s max hp increased by %f!\n", target->name, buff_amount);
                    } else {
                        printf("%s max hp decreased by %f!\n", target->name, buff_amount);
                    }
                    break;
                case ATK:
                    if (skill->effect.is_percentile) {
                        buff_amount = (skill->effect.value * target->atk) - target->atk;
                    } else {
                        buff_amount = skill->effect.value;
                    }
                    target->atk += buff_amount;
                    if (skill->type == BUFF) {
                        printf("%s attack increased by %f!\n", target->name, buff_amount);
                    } else {
                        printf("%s attack decreased by %f!\n", target->name, buff_amount);
                    }
                    break;
                default:
                    break;
            }
            break;
    }
}

// Checks and applies the modifiers inside condition struct of skills
int pass_condition(Condition condition, Character *user) {
    int pass = 0;
    int damage_amount;
    Effect current_effect = condition.effect;
    switch (condition.type) {
        case DAMAGE:
            // Decreases health by an amount, ignore def
            if (condition.is_percentile) {
                damage_amount = (condition.value * user->hp) - user->hp;  
            } else {
                damage_amount = condition.value;
                if (damage_amount >= user->hp) {
                    printf("Can't use skill, %s's hp is too low to use it.\n");
                    return 0;
                }
            }
            printf("%s's hp was reduced by %f!. \n", user->name, damage_amount);
            break;
        case DEBUFF:
            switch (current_effect.type) {
                float debuff_amount;
                case DEF:
                    if (current_effect.is_percentile) {
                        debuff_amount = (current_effect.value * user->def) - user->def;
                    } else {
                        debuff_amount = current_effect.value;
                    }
                    user->def += debuff_amount;
                    printf("%s's defence was decreased by %f", user->name, debuff_amount);
                    break;
                case HP:
                    break;
                case ATK:
                    break;
                default:
                    break;
                }
                break;
        default:
            break;
    }
    return pass;
}

// Function to select the target of the skill
void target_skill(Skill *skill, Character *user, Character *characters, int target_index) {
    if (skill->remaining_cooldown > 0) {
        printf("Skill %s is still in cooldown: %d.\n", skill->name, skill->remaining_cooldown);
        return;
    }

    switch (skill->target) {
        case SELF:
            break;
        case TARGET:
            break;
        case CROWD_SELF:
            break;
        case CROWD_TARGET:
            break;
        default:
            break;
    }
}

// Load Skill from Json
void load_skill(const char *filename, Skill *skills) {

    // Open json file
    cJSON *json = create_json("skill.json");
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

        // Read sub skill
        cJSON *condition = cJSON_GetObjectItem(skill_json, "condition");
        skills[i].condition.type = cJSON_GetObjectItem(condition, "type")->valueint;
        skills[i].condition.target = cJSON_GetObjectItem(condition, "target")->valueint;
        skills[i].condition.value = cJSON_GetObjectItem(condition, "value")->valuedouble;
        skills[i].condition.is_percentile = cJSON_GetObjectItem(condition, "is_percentile")->valueint;
        
        // Read condition effect
        cJSON *condition_effect = cJSON_GetObjectItem(condition, "effects");
        skills[i].condition.effect.type = cJSON_GetObjectItem(condition_effect, "type")->valueint;
        skills[i].condition.effect.value = cJSON_GetObjectItem(condition_effect, "value")->valuedouble;
        skills[i].condition.effect.is_percentile = cJSON_GetObjectItem(condition_effect, "is_percentile")->valueint;
    }
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

///////////                                                                                 ///////////
///////////                     Stack for Time Strike                                       ///////////
///////////                                                                                 ///////////
/* Check if the stack is empty */
int isempty(){
   if(top == -1)
      return 1;
   else
      return 0;
}

/* Check if the stack is full */
int isfull(){
   if(top == MAX_STACK_SIZE)
      return 1;
   else
      return 0;
}

/* Function to return the topmost element in the stack */
int peek(){
   return stack[top];
}

/* Function to delete from the stack */
int pop(){
   int data;
   if(!isempty()) {
      data = stack[top];
      top = top - 1;
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
      return 0;
   }
}

/* Function to insert into the stack */
void push(int data){
   if(!isfull()) {
      top = top + 1;
      stack[top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

/* Function to choose a random skill in the stack*/
int random_Time_Strike(){
    int k_th = rand() % top;
    int id;
    for(int i = 0; i < k_th;i++) {
        id = pop();
    }
    return id;
}
/*  Allows access to the history of moves executed by the player (which is a stack) 
    and randomly selects the k-th move executed counting from the last one          */
void TimeStrike(int id,Character *user, Character *target){
    
    if (!isempty()&&time_strike_use == 1){
        Skill TimeStrike;
        TimeStrike.id = id;
        // Open json file
        cJSON *json = create_json("skill.json");
        // Read skills array
        cJSON *skill_json = cJSON_GetArrayItem(json, TimeStrike.id);

        // Read skills
        strncpy(TimeStrike.name, cJSON_GetObjectItem(skill_json, "name")->valuestring, NAME_LENGTH);
        strncpy(TimeStrike.desc, cJSON_GetObjectItem(skill_json, "description")->valuestring, DESCRITPION_LENGTH);
        TimeStrike.type = cJSON_GetObjectItem(skill_json, "type")->valueint;
        TimeStrike.target = cJSON_GetObjectItem(skill_json, "target")->valueint;
        TimeStrike.value = cJSON_GetObjectItem(skill_json, "value")->valuedouble;
        TimeStrike.is_percentile = cJSON_GetObjectItem(skill_json, "is_percentile")->valueint;
        TimeStrike.cooldown = cJSON_GetObjectItem(skill_json, "cooldown")->valueint;

        // Read effect
        cJSON *effect_json = cJSON_GetObjectItem(skill_json, "effects");
        TimeStrike.effect.type = cJSON_GetObjectItem(effect_json, "type")->valueint;
        TimeStrike.effect.value = cJSON_GetObjectItem(effect_json, "value")->valuedouble;
        TimeStrike.effect.duration = cJSON_GetObjectItem(effect_json, "duration")->valueint;

        // Read sub skill
        cJSON *condition = cJSON_GetObjectItem(skill_json, "condition");
        TimeStrike.condition.type = cJSON_GetObjectItem(condition, "type")->valueint;
        TimeStrike.condition.target = cJSON_GetObjectItem(condition, "target")->valueint;
        TimeStrike.condition.value = cJSON_GetObjectItem(condition, "value")->valuedouble;
        TimeStrike.condition.is_percentile = cJSON_GetObjectItem(condition, "is_percentile")->valueint;
        
        // Read condition effect
        cJSON *condition_effect = cJSON_GetObjectItem(condition, "effects");
        TimeStrike.condition.effect.type = cJSON_GetObjectItem(condition_effect, "type")->valueint;
        TimeStrike.condition.effect.value = cJSON_GetObjectItem(condition_effect, "value")->valuedouble;
        TimeStrike.condition.effect.is_percentile = cJSON_GetObjectItem(condition_effect, "is_percentile")->valueint;
    
        cJSON_Delete(json);
        // Use only one time every battle
        time_strike_use = 0;
        // TimeStrike has 2 times of original value
        TimeStrike.value *= 2;
        TimeStrike.effect.value *= 2;

        // Need to apply skill
    }
    else{
        printf("Cant use TimeStrike!\n");
        return;
    }
}
///////////                                                                                 ///////////
///////////                     Stack for Time Strike                                       ///////////
///////////                                                                                 ///////////
int main(){
    Skill skill_array[MAX_SKILL_IN_GAME];
    skill_array[0].id = 0;
    skill_array[1].id = 1;
    skill_array[2].id = 2;
    skill_array[3].id = 3;
    load_skill("skill.json", skill_array);
    //printf("here\n");
    for (int i = 0; i < MAX_SKILL; i++){
        printf("Skill name: %s\n", skill_array[i].name);
    }
}


/*
// return description of the skill from json file
const char *description(char temp_skill[NAME_LENGTH],char temp_description[DESCRITPION_LENGTH]){

    // open json file
    cJSON *json = create_json("skill.json");

    // access the JSON data 
    strcpy(temp_description,cJSON_GetObjectItem(json,temp_skill)->valuestring);

    // return the desciption
    cJSON_Delete(json);
    return temp_description;
}

// return code of the skill from json file, will be used in initialize_skill function
int skill_code(char temp_description[DESCRITPION_LENGTH]){

    // open json file
    cJSON *json = create_json("skill.json");

    // access the JSON data 
    int code = cJSON_GetObjectItem(json, temp_description)->valueint;

    // return code
    cJSON_Delete(json);
    return code;
}

// decode and initialize the data in skill_array
Skill *decode(Skill skill_array[MAX_SKILL],int code,int i){
    skill_array[i].type = code % 10;
    skill_array[i].value = (code/10)%100;
    skill_array[i].cooldown = code/1000;
    return skill_array;
}

// Initialize the skill from json to the data in the game
void initialize_skill(Skill skill_array[MAX_SKILL]){
    char temp_description[DESCRITPION_LENGTH];
    int code;

    for(int i = 0; i<MAX_SKILL; i++){
        strcpy(skill_array[i].desc,description(skill_array[i].name,temp_description));
        code = skill_code(skill_array[i].desc);
        skill_array = decode(skill_array,code,i);
    }
}

// This function changes the skill, player can choose at most four skill
void skill_change(char All_skill[MAX_SKILL_IN_GAME][NAME_LENGTH],Skill skill_array[MAX_SKILL],int current_skill){
    // Show the Skill can be choosed
    char temp_description[DESCRITPION_LENGTH];
    for(int i = 0;i<current_skill;i++){
        strcpy(temp_description,description(All_skill[i],temp_description));
        printf("%d.%s\nDescription of skill: %s",i+1,All_skill[i],temp_description);
        printf("\n\n");
    }
    printf("Please choose a skill that you want to use in the battle:\n");
    int choosen_skill = check_input(1,current_skill);
    // Show the Skill can be changed
    for(int i = 0; i<MAX_SKILL;i++){
        printf("%d.%s\nDescription of skill: ",i+1,skill_array[i].name);
        printf("%s",description(skill_array[i].name,temp_description));
        printf("\n\n");
    }
    printf("Please choose a skill that you want to change:\n");
    int changed_skill = check_input(1,MAX_SKILL);

    // Change the skill
    strcpy(skill_array[changed_skill-1].name,All_skill[choosen_skill-1]);
    initialize_skill(skill_array);

    // Show the result
    printf("The skill of the player is changed correctly:\n");
    for(int i = 0; i<MAX_SKILL;i++){
        printf("%d.%s\nDescription of skill: ",i+1,skill_array[i].name);
        printf("%s",description(skill_array[i].name,temp_description));
        printf("\n\n");
    }
}
// This function will return the cooldown and execute the skill
int skill_use(int enemies_number,Character user,Character enemies[enemies_number],int skill_choosen){
    int type = user.skill_array->type;
    float value = user.skill_array[skill_choosen].value/10.0;
    int turn = user.skill_array->cooldown;
    int target;
    // Print the name of the skill
    printf("\n%s uses %s\n",user.name,user.skill_array[skill_choosen].name);
    // Each skill is distinguished by the different turn it has
    switch(type){
        case 0:
            // The target is always the user
            // Increase DEF for 2 turn
            if (turn == 2) user.def *=  value; 
            // Increase ATK for 2 turn
            else if (turn == 3) user.atk *= value;
            // Heal
            else user.hp += value * (user.hp_limit - user.hp);
            break;
        case 1:
            // This is atack skill, can choose the target
            if(turn == 1){
                target = check_input(1,enemies_number);
                enemies[target-1].hp -= damage(value*user.atk,enemies[target-1].def);
            }
            else{
                for(int i = 0;i<enemies_number;i++){
                    enemies[i].hp -= damage(value*user.atk,enemies[i].def);
                }
            }
            break;
        case 2:
            // The unique skill wind field
            for(int i = 0;i<enemies_number;i++){
                enemies[i].hp -= value * enemies[i].hp_limit;
            }
            user.hp -= value  * user.hp_limit;
            break;
        case 3:
            // The unique skill last stardust
            target = check_input(1,enemies_number);
            enemies[target].hp -= damage(value*user.atk,enemies[target].def);
            user.hp -= value * user.hp;
            break;
    }
    return turn;
}

int main(){
    // This skill_array is for testing
    Skill skill_array[MAX_SKILL];
    strcpy(skill_array[0].name,All_skill[0]);
    strcpy(skill_array[1].name,All_skill[1]);
    strcpy(skill_array[2].name,All_skill[2]);
    strcpy(skill_array[3].name,All_skill[3]);
    // This value is for limit the maximum amount of skill before the player past the each scenarrio
    // The number of skill will be increased after player win(new skill)
    int current_skill = 8;

    skill_change(All_skill,skill_array,current_skill);

    // This function will return 
    // skill_use(All_skill[0]);
}
*/