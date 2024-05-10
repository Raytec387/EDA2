#include "common.h"
#include "character.h"
#include "common.c"



const char *description(char temp_skill[NAME_LENGTH],char temp_description[DESCRITPION_LENGTH]){

    cJSON *json = create_json("skill.json");

    // access the JSON data 
    cJSON *description = cJSON_GetObjectItemCaseSensitive(json, temp_skill); 
    strcpy(temp_description,description->valuestring);

    // return the desciption
    cJSON_Delete(json);
    return temp_description;

}

int skill_code(char temp_description[DESCRITPION_LENGTH]){
    cJSON *json = create_json("skill.json");
    cJSON *temp_code = cJSON_GetObjectItemCaseSensitive(json, temp_description);

    int code = temp_code->valueint;

    cJSON_Delete(json);
    return code;
}

// Initialize the skill from json to the data in the game
void initialize_skill(Skills skill_array[MAX_SKILL]){
    char temp_description[DESCRITPION_LENGTH];
    int code;
    cJSON *json = create_json("skill.json");
    cJSON *temp_code[MAX_SKILL];

    for(int i = 0; i<MAX_SKILL; i++){
        strcpy(skill_array[i].desc,description(skill_array[i].name,temp_description));

        temp_code[i] = cJSON_GetObjectItemCaseSensitive(json, skill_array[i].desc);
        printf("%d",temp_code[i]->valueint);

    }
    cJSON_Delete(json);
}

// This function is for change the skill, player can choose at most four skill
void skill_change(char All_skill[MAX_SKILL_IN_GAME][NAME_LENGTH],Skills skill_array[MAX_SKILL],int current_skill){
    // Show the skills can be choosed
    char temp_description[DESCRITPION_LENGTH];
    for(int i = 0;i<current_skill;i++){
        strcpy(temp_description,description(All_skill[i],temp_description));
        printf("%d.%s\nDescription of skill: %s",i+1,All_skill[i],temp_description);
        printf("\n\n");
    }
    printf("\nPlese choose a skill that you want to use in the battle.\n");

    // Show the skills can be changed
    /*for(int i = 0; i<MAX_SKILL;i++){
        printf("%d.%s\nDescription of skill: ",i+1,skill_array[i].name);
        description(skill_array[i].name);
        
    }*/


}

int main(){

    char All_skill[MAX_SKILL_IN_GAME][NAME_LENGTH] = {{"Defence"},{"Strong Hit"},{"Heal"},{"Concentrate"},{"Heal+"},{"Good Swordart"},{"Wind Field"},{"Last Stardust"}};
    
    // This skill_array is for testing
    Skills skill_array[MAX_SKILL];
    strcpy(skill_array[0].name,All_skill[0]);
    strcpy(skill_array[1].name,All_skill[1]);
    strcpy(skill_array[2].name,All_skill[2]);
    strcpy(skill_array[3].name,All_skill[3]);
    initialize_skill(skill_array);

    // This value is for limit the maximum amount of skill before the player past the each scenarrio
    // The number of skill will be increased after player win(new skill)
    int current_skill = 8;

    skill_change(All_skill,skill_array,current_skill);
    // This function will return 
    //skill_use(All_skill[0]/*,Enemies,main character*/);
    
}