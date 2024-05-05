#include <stdio.h>

char scenario_3_txt(){
    FILE *fp;
    fp = fopen("3.scenario.txt", "r");

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

void scenario_3_end_txt(){
    FILE *fp;
    fp = fopen("3.scenario.txt", "r");

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

int main(){
    
    scenario_3_txt(); // return a char type function
    /*
    Battle system here
    */
    scenario_3_end_txt(); // return nothing just show the plot
}