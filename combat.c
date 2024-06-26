#include "combat.h"

// Combat menu
void displayBattleOption(){
    printf("1.-Basic atack\n");
    printf("2.-Use a skill\n");
    printf("3.-Time Strike\n");
}

// Damage calculation - damage vs defense
float damage(float damage, int def){
    return (damage * 100/(100+def));
}

// Healing function
void heal(float value, Character *character) {
    if (character->hp + value > character->hp_limit) {
        character->hp = character->hp_limit;
    } else {
        character->hp += value;
    }
}

void apply_active_effect(Character *character) {
    for (int i = 0; i < MAX_ACTIVE_EFFECTS; i++){
        Effect *current_effect = &character->active_effects[i];
        if (current_effect->duration > 0){
            current_effect->duration--;
            switch (current_effect->type) {
                case DMG_OVER_TIME:
                    character->hp -= damage(current_effect->value, character->def);
                    printf("%s suffers %.2f of damage from ongoing effects\n", character->name, damage(current_effect->value, character->def));
                    break;
                case HEAL_OVER_TIME:
                    character->hp += current_effect->value;
                    printf("%s heals for %.2f from ongoing effects\n", character->name, current_effect->value);
                case DEF:
                    /* Solution: Apply buff at skill usage*/
                    /* Remove buff here once its duration is 0*/
                    if (current_effect->duration <= 0) {
                        if (current_effect->is_percentile) {
                            character->def /= current_effect->value;
                        } else {
                            character->def -= current_effect->value;
                        }
                        printf("Effect affecting %s's defence has worn out!\n", character->name);
                    }
                    break;
                case HP:
                    if (current_effect->duration <= 0) {
                        if (current_effect->is_percentile) {
                            character->hp_limit /= current_effect->value;
                        } else {
                            character->hp_limit -= current_effect->value;
                        }
                        printf("Effect affecting %s's max hp has worn out!\n", character->name);
                    }
                case ATK:
                    if (current_effect->duration <= 0) {
                        if (current_effect->is_percentile) {
                            character->atk /= current_effect->value;
                        } else {
                            character->atk -= current_effect->value;
                        }
                        printf("Effect affecting %s's attack has worn out!\n", character->name);
                    }
                default:
                    break;
            }
        }
    }
}

void apply_cooldown (int idx_skill, Turn_node *node) {
    Skill *skill = node->available_Skill[idx_skill];

    // Remove current skill in available skills (moves it outside of num_skill)
    skill->remaining_cooldown = skill->cooldown;

    node->num_skill--;
    Skill *temp = node->available_Skill[idx_skill];
    node->available_Skill[idx_skill] = node->available_Skill[node->num_skill];
    node->available_Skill[node->num_skill] = temp;
}
 
// Function to apply the actual skill
void apply_skill(int idx_skill, Turn_node *node, Character *target) {
    float heal_amount;
    float damage_amount;
    Character *user = node->character;
    Skill *skill = node->available_Skill[idx_skill];

    printf("%s uses %s on %s\n", user->name, skill->name, target->name);

    switch (skill->type) {
        case HEAL:
            if (skill->is_percentile) {
                heal_amount = skill->value * (target->hp_limit - target->hp); // n% of missing
            } else {
                heal_amount = skill->value;
            }
            heal(heal_amount, target);
            printf("%s healed for %.2f hp!\n", target->name, heal_amount);
            break;
        case DAMAGE:
            if (skill->is_percentile) {
                damage_amount = skill->value * user->atk;
            } else {
                damage_amount = skill->value;
            }
            damage_amount = damage(damage_amount, target->def);
            if (damage_amount > target->hp) {
                printf("%s defeated %s\n", user->name, target->name);
                target->hp = 0;
            } else {
                printf("%s dealt %.2f damage to %s\n", user->name, damage_amount, target->name);
                target->hp -= damage_amount;
            }
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
                        printf("%s defence increased by %.2f!\n", target->name, buff_amount);
                    } else {
                        printf("%s defence decreased by %.2f!\n", target->name, buff_amount);
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
                        printf("%s max hp increased by %.2f!\n", target->name, buff_amount);
                    } else {
                        printf("%s max hp decreased by %.2f!\n", target->name, buff_amount);
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
                        printf("%s attack increased by %.2f!\n", target->name, buff_amount);
                    } else {
                        printf("%s attack decreased by %.2f!\n", target->name, buff_amount);
                    }
                    break;
                default:
                    break;
            }
            break;
    }

    // Apply skill effects
    if (skill->effect.duration > 0) {
        for (int i = 0; i < MAX_ACTIVE_EFFECTS; i++) {
            if (target->active_effects[i].duration <= 0) {
                switch (skill->effect.type) {
                    case DMG_OVER_TIME:
                        target->active_effects[i].type = skill->effect.type;
                        target->active_effects[i].duration = skill->effect.duration;
                        target->active_effects[i].value = skill->effect.value;
                        target->active_effects[i].is_percentile = skill->effect.is_percentile;
                        printf("%s is being afflicted with ongoing damage!\n", target->name);
                        break;
                    case HEAL_OVER_TIME:
                        target->active_effects[i].type = skill->effect.type;
                        target->active_effects[i].duration = skill->effect.duration;
                        target->active_effects[i].value = skill->effect.value;
                        target->active_effects[i].is_percentile = skill->effect.is_percentile;
                        printf("%s is being afflicted with ongoing healing!\n", target->name);
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
                break;
            }
        }   
    }
}

void update_cooldowns (Turn_node *node) {
    for (int i = 0; i < MAX_SKILL; i++) {
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

void load_TimeStrike(Skill* TimeStrike){
        TimeStrike->id;
        // Open json file
        cJSON *json = create_json("skill.json");
        // Read skills array
        cJSON *skill_json = cJSON_GetArrayItem(json, TimeStrike->id);

        // Read skills
        strncpy(TimeStrike->name, cJSON_GetObjectItem(skill_json, "name")->valuestring, NAME_LENGTH);
        strncpy(TimeStrike->desc, cJSON_GetObjectItem(skill_json, "description")->valuestring, DESCRITPION_LENGTH);
        TimeStrike->type = cJSON_GetObjectItem(skill_json, "type")->valueint;
        TimeStrike->target = cJSON_GetObjectItem(skill_json, "target")->valueint;
        TimeStrike->value = cJSON_GetObjectItem(skill_json, "value")->valuedouble;
        TimeStrike->is_percentile = cJSON_GetObjectItem(skill_json, "is_percentile")->valueint;
        TimeStrike->cooldown = cJSON_GetObjectItem(skill_json, "cooldown")->valueint;

        // Read effect
        cJSON *effect_json = cJSON_GetObjectItem(skill_json, "effects");
        TimeStrike->effect.type = cJSON_GetObjectItem(effect_json, "type")->valueint;
        TimeStrike->effect.value = cJSON_GetObjectItem(effect_json, "value")->valuedouble;
        TimeStrike->effect.is_percentile = cJSON_GetObjectItem(effect_json, "is_percentile")->valueint;
        TimeStrike->effect.duration = cJSON_GetObjectItem(effect_json, "duration")->valueint;
    
        cJSON_Delete(json);
        // TimeStrike has 2 times of original value
        TimeStrike->value *= 2;
        TimeStrike->effect.value *= 2;
}

void execute_TimeStrike(Skill* skill,Character *user,Character *target){
    float heal_amount;
    float damage_amount;

    printf("%s uses %s on %s\n", user->name, skill->name, target->name);

    switch (skill->type) {
        case HEAL:
            if (skill->is_percentile) {
                heal_amount = skill->value * (target->hp_limit - target->hp); // n% of missing
            } else {
                heal_amount = skill->value;
            }
            heal(heal_amount, target);
            printf("%s healed for %.2f hp!\n", target->name, heal_amount);
            break;
        case DAMAGE:
            if (skill->is_percentile) {
                damage_amount = skill->value * user->atk;
            } else {
                damage_amount = skill->value;
            }
            damage_amount = damage(damage_amount, target->def);
            if (damage_amount > target->hp) {
                printf("%s defeated %s\n", user->name, target->name);
                target->hp = 0;
            } else {
                printf("%s dealt %.2f damage to %s\n", user->name, damage_amount, target->name);
                target->hp -= damage_amount;
            }
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
                        printf("%s defence increased by %.2f!\n", target->name, buff_amount);
                    } else {
                        printf("%s defence decreased by %.2f!\n", target->name, buff_amount);
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
                        printf("%s max hp increased by %.2f!\n", target->name, buff_amount);
                    } else {
                        printf("%s max hp decreased by %.2f!\n", target->name, buff_amount);
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
                        printf("%s attack increased by %.2f!\n", target->name, buff_amount);
                    } else {
                        printf("%s attack decreased by %.2f!\n", target->name, buff_amount);
                    }
                    break;
                default:
                    break;
            }
            break;
    }

    // Apply skill effects
    if (skill->effect.duration > 0) {
        for (int i = 0; i < MAX_ACTIVE_EFFECTS; i++) {
            if (target->active_effects[i].duration <= 0) {
                switch (skill->effect.type) {
                    case DMG_OVER_TIME:
                        target->active_effects[i].type = skill->effect.type;
                        target->active_effects[i].duration = skill->effect.duration;
                        target->active_effects[i].value = skill->effect.value;
                        target->active_effects[i].is_percentile = skill->effect.is_percentile;
                        printf("%s is being afflicted with ongoing damage!\n", target->name);
                        break;
                    case HEAL_OVER_TIME:
                        target->active_effects[i].type = skill->effect.type;
                        target->active_effects[i].duration = skill->effect.duration;
                        target->active_effects[i].value = skill->effect.value;
                        target->active_effects[i].is_percentile = skill->effect.is_percentile;
                        printf("%s is being afflicted with ongoing healing!\n", target->name);
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
                break;
            }
        }   
    }
}
/*  Allows access to the history of moves executed by the player (which is a stack) 
    and randomly selects the k-th move executed counting from the last one          */
void apply_TimeStrike(int id,Game_state *currentState,Turn_queue *queue) {
    Skill skill;
    skill.id = id;
    load_TimeStrike(&skill);
    printf("\n%s uses TimeStrike(based on %s)\n", currentState->character.name, skill.name);

    int target;

    switch (skill.target) {
        case SELF:
            execute_TimeStrike(&skill,&currentState->character,&currentState->character);
            break;

        case TARGET:
            printf("Please select an enemy to use the skill on.\n");
            display_enemies(queue);
            target = check_input(1, queue->size - 1);
            execute_TimeStrike(&skill,&currentState->character,queue->enemies[target - 1]);
            break;

        case CROWD_SELF:
            // Player character is always alone :(
            break;

        case CROWD_TARGET:
            for (int i = 0; i < queue->size - 1; i++) {
                execute_TimeStrike(&skill,&currentState->character,queue->enemies[i]);
            }
            break;

        default:
            break;
    }
}

// Function to choose a random skill in the stack
int random_Time_Strike(Character *character) {
    int k_th = rand() % character->ability_stack.top+1;
    int id;
    for(int i = 0; i < k_th;i++) {
        id = pop(character);
    }
    return id;
}

///////////                                                                                 ///////////
///////////                     Stack for Time Strike                                       ///////////
///////////                                                                                 ///////////
/* Check if the stack is empty */
int isempty(Character *character) {
   if (character->ability_stack.top == -1)
      return 1;
   else
      return 0;
}

/* Check if the stack is full */
int isfull(Character *character) {
   if(character->ability_stack.top == MAX_STACK_SIZE)
      return 1;
   else
      return 0;
}

/* Function to return the topmost element in the stack */
int peek(Character *character) {
   return character->ability_stack.stack[character->ability_stack.top];
}

/* Function to delete from the stack */
int pop(Character *character) {
   int data;
   if(!isempty(character)) {
      data = character->ability_stack.stack[character->ability_stack.top];
      character->ability_stack.top--;
      return data;
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
      return 0;
   }
}

/* Function to insert into the stack */
void push(Character *character, int data) {
    if(!isfull(character)) {
        character->ability_stack.top++;
        character->ability_stack.stack[character->ability_stack.top] = data;
    } 
    else {
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

void init_Tqueue(Turn_queue *queue, Character *player, Character *enemies[], int n_enemies) {
    int size = n_enemies + 1;
    queue->player = player;
    queue->num_turns = 1;

    for (int i = 0; i < n_enemies; i++) {
        if (enemies[i] != NULL) {
            queue->enemies[i] = enemies[i];
        }
    }
    
    //Randomize queue
    int random_queue[size];
    
    for (int i = 0; i < size; i++) {
        random_queue[i] = i;
    }

    for (int i = 0; i < size * 2; i++) {
        int a = rand() % size;
        int b = rand() % size;
        int temp = random_queue[a];
        random_queue[a] = random_queue[b];
        random_queue[b] = temp;
    }

    //Assign character to respective turns
    for (int i = 0; i < size; i++) {
        
        int idx = -1;
        //Find index of the corresponding character
        for (int j = 0; j < size; j++) {
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

        if (i == size - 1) {
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
void dequeue(Turn_queue *queue) {
    Turn_node *temp = queue->head;
    if (queue->head == queue->tail) {
        queue->head = NULL;
        queue->tail = NULL;
    } else {
        queue->head = queue->head->next;
    }
    temp->next = NULL;
    queue->size--;
}

void display_enemies(Turn_queue *queue) {
    printf("\nEnemies:\n");
    for (int i = 0; i < queue->size - 1; i++) {
        if (queue->enemies[i] != NULL) {
            printf("%d. %s: HP [%.2f/%.2f]\n", (i + 1), queue->enemies[i]->name, queue->enemies[i]->hp, queue->enemies[i]->hp_limit);
        }
    }
    printf("\n");
}

void display_battle(Turn_queue *queue) {
    Character *player = queue->player;
    printf("\n== ONGOING BATTLE ==\n");
    printf("Turn: %d\n", queue->num_turns);
    printf("%s: HP [%.2f/%.2f]\n", player->name, player->hp, player->hp_limit);
    display_enemies(queue);
}

void display_skills(Turn_node *node) {
    printf("\nAvailable skills: \n");
    for (int i = 0; i < node->num_skill; i++) {
        printf("%d. %s\n", (i + 1), node->available_Skill[i]->name);
        printf("\"%s\"\n", node->available_Skill[i]->desc);
        printf("\n");
    }
    printf("Abilities on cooldown: \n");
    for (int i = node->num_skill; i < MAX_SKILL; i++) {
        printf("%d. %s. Cooldown: %d turns\n", (i + 1), node->available_Skill[i]->name, node->available_Skill[i]->remaining_cooldown);
        printf("\"%s\"\n", node->available_Skill[i]->desc);
        printf("\n");
    }
}

void remove_enemy(Turn_queue *queue, Character *enemy, int index) {
    Turn_node *current_node = queue->head;
    Turn_node *temp;

    // Remove node from queue
    if (current_node->character == enemy) {
        temp = current_node;
        dequeue(queue);
    } else {
        while (current_node->next != NULL) {
            if (current_node->next->character == enemy) {
                temp = current_node->next;
                current_node->next = temp->next;
                if (temp == queue->tail) {
                    queue->tail = current_node;
                }
                queue->size--;
                break;
            } else {
                current_node = current_node->next;
            }
        }
    }

    for (int i = index; i < queue->size; i++) {
        // Move enemies down one index
        queue->enemies[i] = queue->enemies[i + 1];
    }
    free(temp->character);
    free(temp);
}

void player_turn(Turn_node *node, Turn_queue *queue, Game_state *current_state) {
    int choice;
    int skill_idx;
    int target;
    bool turn_done = false;
    Character *player = queue->player;

    // Main loop handling the turn
    do {
        displayBattleOption();
        choice = check_input(USE_ATTACK, USE_TIME_STRIKE);
        
        switch (choice) {
            case USE_ATTACK:
                printf("Who do you want to attack?\n");
                display_enemies(queue);
                target = check_input(0, queue->size);

                if (target != 0) {
                    float damage_amount = damage(player->atk, queue->enemies[target - 1]->def);
                    queue->enemies[target - 1]->hp -= damage_amount;

                    if (queue->enemies[target - 1]->hp < 0.0) {
                        queue->enemies[target - 1]->hp = 0.0;
                        printf("%s was defeated by %s!\n", queue->enemies[target - 1]->name, player->name);
                    } else {
                        printf("%s dealt %.2f damage to %s!\n", player->name, damage_amount, queue->enemies[target - 1]->name);
                    }
                    turn_done = true;
                }
                break;

            case USE_SKILL:
                printf("Which skill do you want to use?\n");
                printf("0.-Exit\n");
                display_skills(node);
                skill_idx = check_input(0, node->num_skill);
            
                if (skill_idx != 0) {
                    Skill *skill = node->available_Skill[skill_idx - 1];
                    int target;

                    switch (skill->target) {
                        case SELF:
                            apply_skill(skill_idx - 1, node, player);
                            break;

                        case TARGET:
                            printf("Please select an enemy to use the skill on.\n");
                            display_enemies(queue);
                            target = check_input(1, queue->size - 1);
                            apply_skill(skill_idx - 1, node, queue->enemies[target - 1]);
                            break;

                        case CROWD_SELF:
                            // Player character is always alone :(
                            break;

                        case CROWD_TARGET:
                            for (int i = 0; i < queue->size - 1; i++) {
                                apply_skill(skill_idx - 1, node, queue->enemies[i]);
                            }
                            break;

                        default:
                            break;
                    }
                    apply_cooldown(skill_idx - 1, node);
                    // For time-strike and dictionary
                    use_ability(player->tracker, skill->name);   
                    push(&current_state->character, skill->id);

                    turn_done = true;
                }
                break;

            case USE_TIME_STRIKE:
                if(current_state->character.ability_stack.time_strike_use == true && !isempty(&current_state->character)){
                    int id = random_Time_Strike(&current_state->character);
                    apply_TimeStrike(id,current_state,queue);
                    current_state->character.ability_stack.time_strike_use = false;
                    turn_done = true;
                } 
                else{
                    printf("Please use TimeStrike correctly!\n");
                }
                break;

            default:
                break;
        }
    } while (!turn_done); 
    
}

// Function that selects random skill
void enemy_skill_use(Turn_node *node, Turn_queue *queue, Game_state *current_state) {
    int option = rand() % 2;
    Character *enemy = node->character;
    Character *player = queue->player;
    
    if (option == 0) {
        float damage_amount = damage(enemy->atk, player->def);
        player->hp -= damage_amount;

        if (player->hp <= 0.0) {
            player->hp = 0.0;
            printf("%s was defeated by %s!\n", player->name, enemy->name);
        } else {
            printf("%s dealt %.2f damage to %s!\n", enemy->name, damage_amount, player->name);
        }

    } else {
        int choice = rand() % node->num_skill;
        Skill *skill = node->available_Skill[choice];
        switch (skill->target) {
            case SELF:
                apply_skill(choice, node, enemy);
                break;
            case TARGET:
                apply_skill(choice, node, player);
                break;
            case CROWD_SELF:
                for (int i = 0; i < queue->size - 1; i++) {
                    apply_skill(choice, node, queue->enemies[i]);
                }
                break;
            case CROWD_TARGET:
                apply_skill(choice, node, player);
                break;
            default:
                break;
        }
        apply_cooldown(choice, node);
    }
}

void reset_player(Character *player) {
    player->hp = player->hp_limit;

    // Reset active effects
    for (int i = 0; i < MAX_ACTIVE_EFFECTS; i++) {
        Effect *current_effect = &player->active_effects[i];
        if (current_effect->duration > 0) {
            current_effect->duration = 0;
            switch (current_effect->type) {
                case DEF:
                    if (current_effect->duration <= 0) {
                        if (current_effect->is_percentile) {
                            player->def /= current_effect->value;
                        } else {
                            player->def -= current_effect->value;
                        }
                    }
                    break;
                case HP:
                    if (current_effect->duration <= 0) {
                        if (current_effect->is_percentile) {
                            player->hp_limit /= current_effect->value;
                        } else {
                            player->hp_limit -= current_effect->value;
                        }
                    }
                case ATK:
                    if (current_effect->duration <= 0) {
                        if (current_effect->is_percentile) {
                            player->atk /= current_effect->value;
                        } else {
                            player->atk -= current_effect->value;
                        }
                    }
                default:
                    break;
            }
        }
    }

    // Reset coolodwns
    for (int i = 0; i < MAX_SKILL; i++) {
        if (player->skill_array[i].remaining_cooldown > 0) {
            player->skill_array[i].remaining_cooldown = 0;
        }
    }
}

bool combat(Character *player, Character *enemies[], Game_state *current_state, int n_enemies) {
    bool outcome = false;
    bool end = false;
    Turn_queue *queue = create_Tqueue();
    init_Tqueue(queue, player, enemies, n_enemies);
    current_state->character.ability_stack.time_strike_use = true;

    while(!end) {
        bool end_turn = false;
        Turn_node *current_node = queue->head;

        do {
            update_cooldowns(current_node);
            apply_active_effect(current_node->character);
            current_node = current_node->next;
        } while (current_node != NULL);

        while (!end_turn) {
            current_node = queue->head;

            if (current_node->is_last == true) {
                end_turn = true;
            }

            printf("\n");
            if (!current_node->character->is_player) {
                enemy_skill_use(current_node, queue, current_state);
            } else {
                display_battle(queue);
                player_turn(current_node, queue, current_state);
            }

            // Move node at the end of the queue
            dequeue(queue);
            enqueue(queue, current_node);

            // Remove any enemy who got defeated
            for (int i = 0; i < queue->size - 1; i++) {
                if (queue->enemies[i]->hp <= 0.0) {
                    remove_enemy(queue, queue->enemies[i], i);
                }
            }

            if (player->hp <= 0.0) {
                end = true;
                outcome = false; // player lost
                end_turn = true;
            } else if (queue->size == 1) {
                end = true;
                outcome = true; // player won
                end_turn = true;
            }
        }
        queue->num_turns++;
        if (queue->num_turns > MAX_TURNS) {
            printf("\nYou have run out of turns.\n");
            end = true;
            outcome = false;
        }
    }

    // remove all enemies
    for (int i = 0; i < queue->size - 1; i++) {
        remove_enemy(queue, queue->enemies[i], i);
    }
    reset_player(player);
    free(queue->head); // Free player node
    free(queue);

    return outcome;
}