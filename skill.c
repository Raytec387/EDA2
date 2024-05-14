#include "common.h"
#include "character.h"

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
Skills *decode(Skills skill_array[MAX_SKILL],int code,int i){
    skill_array[i].type = code % 10;
    skill_array[i].value = (code/10)%100;
    skill_array[i].cooldown = code/1000;
    return skill_array;
}

// Initialize the skill from json to the data in the game
void initialize_skill(Skills skill_array[MAX_SKILL]){
    char temp_description[DESCRITPION_LENGTH];
    int code;

    for(int i = 0; i<MAX_SKILL; i++){
        strcpy(skill_array[i].desc,description(skill_array[i].name,temp_description));
        code = skill_code(skill_array[i].desc);
        skill_array = decode(skill_array,code,i);
    }
}

// This function changes the skill, player can choose at most four skill
void skill_change(char All_skill[MAX_SKILL_IN_GAME][NAME_LENGTH],Skills skill_array[MAX_SKILL],int current_skill){
    // Show the skills can be choosed
    char temp_description[DESCRITPION_LENGTH];
    for(int i = 0;i<current_skill;i++){
        strcpy(temp_description,description(All_skill[i],temp_description));
        printf("%d.%s\nDescription of skill: %s",i+1,All_skill[i],temp_description);
        printf("\n\n");
    }
    printf("Please choose a skill that you want to use in the battle:\n");
    int choosen_skill = check_input(1,current_skill);
    // Show the skills can be changed
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
                int target = check_input(1,enemies_number);
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
            int target = check_input(1,enemies_number);
            enemies[target-1].hp -= damage(value*user.atk,enemies[target-1].def);
            user.hp -= value * user.hp;
            break;
    }
    return turn;
}

int damage(int damage,int def){
    return (damage * 100/(100+def));
}

/*
int main(){
    // This skill_array is for testing
    Skills skill_array[MAX_SKILL];
    strcpy(skill_array[0].name,All_skill[0]);
    strcpy(skill_array[1].name,All_skill[1]);
    strcpy(skill_array[2].name,All_skill[2]);
    strcpy(skill_array[3].name,All_skill[3]);
    // This value is for limit the maximum amount of skill before the player past the each scenarrio
    // The number of skill will be increased after player win(new skill)
    int current_skill = 8;

    skill_change(All_skill,skill_array,current_skill);

    // This function will return 
    //skill_use(All_skill[0]);
}
*/