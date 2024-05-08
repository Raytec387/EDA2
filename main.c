#include "common.h"

void start_game(){

}

void configure_character(){

    displayCharacter_Configuration();
    do{
        printf("What do yoy want to change(%d-%d):"NAME, DEF);
        int option;
        option=read_int();
    }while(option < NAME || option > DEF);

    switch (option){
case NAME:
    printf("Insert the new name of the character:");
    
    break;

case HP:
    printf("Insert the HP of the character:");
    break;
case ATK:
    printf("Instert the ATK of the character:");
    break;
case DEF:
    printf("Instert the DEF of the character:");
    break;

        
            
    

}


void skill_test(){

}
void displayCharacter_Configuration(){
    printf("1.-Change characater name\n");
    printf("2.-Change character HP\n");
    printf("3.-Change characater ATK\n");
    printf("4.-Change characater DEF\n");
}




void displayMenu(){
    printf("===Red Rain===\n");
    printf("1.-Start a new game.\n");
    printf("2.-Configure the character\n");
    printf("3.-Skills\n");
}
void run(){
    int option;
    do{
        displayMenu();
        do{
            printf("select an option 1-3\n");
            option=read_int();
        }while(option < START_GAME || option> CONFIGURE_cHARACTER);
        switch(option){
            case START_GAME:
                start_game();
                break;
            case CONFIGURE_CHARACTER:
                configure_character();
                break;
        }
            

    }


}

int main(){
    run();
}
