#include "common.h"

void start_game(){

}

void configure_character(){
    printf("1.-Change characater name\n");
    printf("2.-Change character HP\n");
    printf("3.-Change characater ATK\n");
    printf("4.-Change characater DEF\n");
    printf("What do yoy want to change(1-4)?"\n);
    int option;
    option=read_int();
    if option==1{
        printf("Insert the new name of the character:");
    }
    if option==2{
        printf("Insert the HP of the character:");
    }

    if option==3{
        printf("Instert the ATK of the character:");
    }
    if option==4{
        printf("INster the DEF of the character:");
        Main_character.
        
        
            
    

}


void skill_test(){

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
        }
        if option==1{
            start_game();
        }
        if option==2{
            configure_character();
        }
        if option==3{
            skill_test();
                }
    }


}

int main(){
    run();
}
