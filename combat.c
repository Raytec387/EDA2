#include "combat.h"


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


// Function to apply the actual skill
void apply_skill(Skill *skill, Character *user, Character *target) {
    float heal_amount;
    float damage_amount;

    /*
    // For dictionary and Timestrike(with stack)
    if(user->is_player) {
        use_ability(currentState->tracker, skill->name);   
        push(skill->id);
    }
    */

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

    // Apply skill effects
    for (int i = 0; i < MAX_ACTIVE_EFFECTS; i++) {
        if (target->active_effects[i].duration <= 0) {
            switch (skill->effect.type) {
            case DMG_OVER_TIME:
                target->active_effects[i].type = skill->effect.type;
                target->active_effects[i].duration = skill->effect.duration;
                target->active_effects[i].value = skill->effect.value;
                target->active_effects[i].is_percentile = skill->effect.is_percentile;
                printf("%s is being afflicted with ongoing damage!", target->name);
                break;
            case HEAL_OVER_TIME:
                target->active_effects[i].type = skill->effect.type;
                target->active_effects[i].duration = skill->effect.duration;
                target->active_effects[i].value = skill->effect.value;
                target->active_effects[i].is_percentile = skill->effect.is_percentile;
                printf("%s is being afflicted with ongoing healing!", target->name);
                break;
            case DEF:
            case HP:
            case ATK:
                target->active_effects[i].type = skill->effect.type;
                target->active_effects[i].duration = skill->effect.duration;
                target->active_effects[i].value = skill->effect.value;
                target->active_effects[i].is_percentile = skill->effect.is_percentile;
                break;
            default:
                break;
            }
        }
    }
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

/*  Allows access to the history of moves executed by the player (which is a stack) 
    and randomly selects the k-th move executed counting from the last one          */
void TimeStrike(int id, Character *user, Character *target){
    
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

// Function to choose a random skill in the stack
int random_Time_Strike(){
    int k_th = rand() % top;
    int id;
    for(int i = 0; i < k_th;i++) {
        id = pop();
    }
    return id;
}

// Function to initialize a node struct
// Returns adress of a node
Turn_node* create_Tnode(Character *character) {
    Turn_node *new_node = malloc(sizeof(*new_node));
    new_node->character = character;
    new_node->next = NULL;
    return new_node;
}

// Function to initialize a queue
// Return adress of a queue
Turn_queue* create_Tqueue() {
    Turn_queue *new_queue = malloc(sizeof(*new_queue));
    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->num_turns = 0;
    new_queue->size = 0;
    return new_queue;
}

// Function to enqueue node to a queue
void enqueue(Turn_queue *queue, Turn_node *node) {
    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    queue->size++;
}

// Function to dequeue node from a queue
// Returns the address of the dequeued node
Turn_node *dequeue(Turn_queue *queue) {
    Turn_node *temp = queue->head;
    if (queue->head == queue->head) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = queue->head->next;
    }
    queue->size--;
    return temp; 
}

int combat(Character *player, Character *characters[]) {
    int end = 0;

    Turn_queue *queue = create_Tqueue();

    while(!end) {
        Turn_node *current_node = dequeue(queue);

        if (!current_node->character->is_player) {
            
        }
    }
}

// Function that selects random skill
void enemy_skill_use(Character *enemy, Character *player){
    int choice = rand() % MAX_SKILL;
    
}