

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
            printf("select an option\n");
            option=read_int();
        }
    }


}

int main(){
    run();
}
