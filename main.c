#include "main.h"

void run(){
    int option;
    do{
        displayMenu();
        do{
            printf("select an option: ");
            scanf("%d", &option);
        }while(option < EXIT_BACK || option > SAVE_GAME);
        
        switch(option){
            case START_GAME:
                start_game();
                break;
            case LOAD_GAME:
                load_game();
                break;
            case SAVE_GAME:
                save_game();
                break;
        }
    } while (option != EXIT_BACK);
}

int main(){
    run();
}
