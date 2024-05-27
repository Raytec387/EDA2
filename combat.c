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

void apply_active_effect(Character *character) {
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
void apply_skill(Skill *skill, Character *user, Character *target, Game_state *currentState) {
    float heal_amount;
    float damage_amount;

    
    // For dictionary and Timestrike(with stack)
    if(user->is_player) {
        use_ability(currentState->tracker, skill->name);   
        push(currentState, skill->id);
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
void target_skill(Skill *skill, Character *user, Character *characters[], int target_index, Game_state *current_state) {
    if (skill->remaining_cooldown > 0) {
        printf("Skill %s is still in cooldown: %d.\n", skill->name, skill->remaining_cooldown);
        return;
    }
    int n_targets = sizeof(characters)/sizeof(user);

    switch (skill->target) {
        case SELF:
            apply_skill(skill, user, user, current_state);
            break;
        case TARGET:
            apply_skill(skill, user, characters[target_index], current_state);
            break;
        case CROWD_SELF:
            // TO DO
            // Since player is always alone, target everyone except player
            // Only enemies would have this type of ability
            break;
        case CROWD_TARGET:
            for (int i = 0; i < n_targets; i++) {
                if (characters[i] != NULL) {
                    apply_skill(skill, user, characters[i], current_state);
                }
            }
            break;
        default:
            break;
    }
}

void update_cooldowns (Turn_node *node) {
    for (int i = node->num_skill; i < MAX_SKILL; i++) {
        if (node->available_Skill[i]->remaining_cooldown > 0) {
            node->available_Skill[i]->remaining_cooldown--;
            if (node->available_Skill[i]->remaining_cooldown <= 0) {
                node->num_skill++;
                Skill *temp = node->available_Skill[i];
                node->available_Skill[i] = node->available_Skill[node->num_skill - 1];
                node->available_Skill[node->num_skill - 1] = temp; 
            }
        }
    }
}

/*  Allows access to the history of moves executed by the player (which is a stack) 
    and randomly selects the k-th move executed counting from the last one          */
void TimeStrike(int id, Character *user, Character *target, Game_state *current_state) {
    
    if (!isempty(current_state) && current_state->ability_stack.time_strike_use == 1){
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
        current_state->ability_stack.time_strike_use = 0;
        // TimeStrike has 2 times of original value
        TimeStrike.value *= 2;
        TimeStrike.effect.value *= 2;

        // Need to apply skill
    }
    else{
        printf("Can't use TimeStrike!\n");
        return;
    }
}

// Function to choose a random skill in the stack
int random_Time_Strike(Game_state *current_state) {
    int k_th = rand() % current_state->ability_stack.time_strike_use;
    int id;
    for(int i = 0; i < k_th;i++) {
        id = pop(current_state);
    }
    return id;
}

///////////                                                                                 ///////////
///////////                     Stack for Time Strike                                       ///////////
///////////                                                                                 ///////////
/* Check if the stack is empty */
int isempty(Game_state *current_state) {
   if (current_state->ability_stack.top == -1)
      return 1;
   else
      return 0;
}

/* Check if the stack is full */
int isfull(Game_state *current_state) {
   if(current_state->ability_stack.top == MAX_STACK_SIZE)
      return 1;
   else
      return 0;
}

/* Function to return the topmost element in the stack */
int peek(Game_state *current_state) {
   return current_state->ability_stack.stack[current_state->ability_stack.top];
}

/* Function to delete from the stack */
int pop(Game_state *current_state) {
   int data;
   if(!isempty(current_state)) {
      data = current_state->ability_stack.stack[current_state->ability_stack.top];
      current_state->ability_stack.top--;
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
      return 0;
   }
}

/* Function to insert into the stack */
void push(Game_state *current_state, int data) {
   if(!isfull(current_state)) {
      current_state->ability_stack.top++;
      current_state->ability_stack.stack[current_state->ability_stack.top] = data;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
}

// Function to initialize a node struct
// Returns adress of a node
Turn_node* create_Tnode() {
    Turn_node *new_node = malloc(sizeof(*new_node));
    return new_node;
}

void init_Tnode(Turn_node *node, Character *character) {
    node->character = character;
    node->next = NULL;
    node->num_skill = MAX_SKILL;
    for (int i = 0; i < MAX_SKILL; i++) {
        node->available_Skill[i] = &character->skill_array[i];
    }
    node->is_last = false;
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

void init_Tqueue(Turn_queue *queue, Character *player, Character *enemies[]) {
    queue->size = 1;
    queue->player = player;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i] != NULL) {
            queue->enemies[i] = enemies[i];
            queue->size++;
        }
    }

    //Randomize queue
    int random_queue[queue->size];
    int count = 0;
    for (int i = queue->size; i > 0; i--) {
        random_queue[count] = rand() % i;
    }

    //Assign character to respective turns
    for (int i = 0; i < queue->size; i++) {
        int idx = -1;
        //Find index of the corresponding character
        for (int j = 0; j < queue->size; j++) {
            if (i == random_queue[j]) {
                idx = j;
                break;
            }
        }

        //Index 0 is the player
        Turn_node *new_node = create_Tnode();
        if (idx == 0) { 
            init_Tnode(new_node, player);
            enqueue(queue, new_node);
        } else {
            init_Tnode(new_node, enemies[idx - 1]);
            enqueue(queue, new_node);
        } 

        if (i == queue->size - 1) {
            new_node->is_last = true;
        }
    }
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
    if (queue->head == queue->tail) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = queue->head->next;
    }
    queue->size--;
    return temp; 
}

int combat(Character *player, Character *enemies[], Game_state *current_state) {
    int end = 0;
    
    Turn_queue *queue = create_Tqueue();

    while(!end) {
        Turn_node *current_node = dequeue(queue);

        if (!current_node->character->is_player) {
            
        }
    }
}

// Function that selects random skill
void enemy_skill_use(Turn_node *node, Character *player, Game_state *current_state) {
    int choice = rand() % node->num_skill;
    target_skill(node->available_Skill[choice], node->character, player, 0, current_state);
    
    // Remove current skill in available skills
    node->num_skill--;
    Skill *temp = node->available_Skill[choice];
    node->available_Skill[choice] = node->available_Skill[node->num_skill - 1];
    node->available_Skill[node->num_skill - 1] = temp;
}