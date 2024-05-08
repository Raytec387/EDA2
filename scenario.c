#include <stdio.h>
#include <string.h>
#define MAX_STR_NAME 20
#define MAX_SCENARIO 5
const char txt[MAX_SCENARIO][MAX_STR_NAME] = {{"1.scenario.txt"},{"2.scenario.txt"},{"3.scenario.txt"},{"4.scenario.txt"},{"4.end.txt"}};

char scenario_txt(char txt[MAX_STR_NAME]){
    FILE *fp;

    fp = fopen(txt, "r");
    printf("\033[2J\033[1;1H");

    char choose;

    if (fp == NULL) {
        printf("The file is not opened. The program will "
               "now exit.");
        fclose(fp);
    }
    else{
        char c = fgetc(fp);
        while(1){
            if (c == '&') {
                scanf("%c", &choose);
                printf("\033[2J\033[1;1H");
                c = fgetc(fp);
            }
            else if(c == '*'){
                break;
            }
            printf("%c", c);
            c = fgetc(fp);

        }

    }
    while (1){
        if (choose == '1'){
            break;
            printf("Here");
        }
        
        else if (choose == '2'){
            break;
        }

        else{
            printf("\n(Please choose 1 or 2.)\n");
            scanf("%c", &choose);
            printf("\033[2J\033[1;1H");
        }
    }
    return choose;
}

void scenario_end_txt(char txt[MAX_STR_NAME]){
    FILE *fp;
    fp = fopen(txt, "r");

    printf("\033[2J\033[1;1H");

    char choose;

    if (fp == NULL) {
        printf("The file is not opened. The program will "
               "now exit.");
        fclose(fp);
    }
    else{
        char c = fgetc(fp);
        while (c != '*')    c = fgetc(fp);
        while(1){
            if (c == '&') {
                scanf("%c", &choose);
                printf("\033[2J\033[1;1H");
                c = fgetc(fp);
            }
            else if(c == EOF){
                break;
            }
            printf("%c", c);
            c = fgetc(fp);

        }
        fclose(fp);
    }
}

// This is for testing, we need to create a main.c file 
int main(){
    // scenario_txt(txt[integer number here]); // return a char type function
    /*
    Battle system here
    */
    // scenario_end_txt(txt[integer number here]); // return nothing just show the plot
}