#include "scenario.h"

//// Graph part ////

// Function to create a scenario
Scenario* createScenario(int id, const char* name) {
    Scenario* newScenario = (Scenario*)malloc(sizeof(Scenario));
    newScenario->id = id;
    snprintf(newScenario->name, sizeof(newScenario->name), "%s", name);
    newScenario->battleWon = false;
    newScenario->adjCount = 0;
    return newScenario;
}

// Function to add an edge (route) between two scenarios
void addEdge(Scenario* src, Scenario* dest) {
    if (src->adjCount < MAX_SCENARIO) {
        src->adjacent[src->adjCount++] = dest;
    }
}

// Function to show adjacent scenarios
void showAdjacentScenarios(Scenario* scenario) {
    printf("Adjacent scenarios:\n");
    for (int i = 0; i < scenario->adjCount; i++) {
        printf("ID: %d, Name: %s\n", scenario->adjacent[i]->id, scenario->adjacent[i]->name);
    }
}

// Function to get a scenario by its ID
Scenario* getScenarioById(Scenario* scenarios[], int id, int count) {
    for (int i = 0; i < count; i++) {
        if (scenarios[i]->id == id) {
            return scenarios[i];
        }
    }
    return NULL;
}

// Free scenario pointer
void freeScenarios(Scenario* scenarios[], int count) {
    for (int i = 0; i < count; i++) {
        if (scenarios[i]) {
            free(scenarios[i]);
        }
    }
}
//// Graph part ////
//// Graph part ////

int scenario_txt(char txt[NAME_LENGTH]){
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
            return 1;
            break;
        }
        else if(choose == '2'){
            // It returns the number of enemies
            return 3;
        }
        else{
            printf("\n(Please choose 1 or 2.)\n");
            scanf("%c", &choose);
            printf("\033[2J\033[1;1H");
        }
    }
    printf("\nError here!\n");
    return -1;
}

void scenario_end_txt(char txt[NAME_LENGTH]){
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

void graph_initialize(Scenario *scenarios[MAX_SCENARIO]){
    
    scenarios[0] = createScenario(1, "Childhood.txt");
    scenarios[1] = createScenario(2, "Travelling.txt");
    scenarios[2] = createScenario(3, "Training.txt");
    scenarios[3] = createScenario(4, "Travelling_II.txt");
    scenarios[4] = createScenario(5, "Ending.txt");

    // Create connections between scenarios
    // Main character can recall the story but cant modify the option he has chosen
    addEdge(scenarios[0], scenarios[1]); // 
    
    addEdge(scenarios[1], scenarios[0]); // 
    addEdge(scenarios[1], scenarios[2]); // 

    addEdge(scenarios[2], scenarios[0]); // 
    addEdge(scenarios[2], scenarios[1]); // 
    addEdge(scenarios[2], scenarios[3]); // 

    addEdge(scenarios[3], scenarios[0]); // 
    addEdge(scenarios[3], scenarios[1]); // 
    addEdge(scenarios[3], scenarios[2]); // 
    addEdge(scenarios[3], scenarios[4]); // 
}

void story_Navi_battleCheck(Scenario *scenarios[MAX_SCENARIO], Game_state *currentState, Scenario* currentScenario){
    /// Story and navigation ///
    while (true) {
        // Ending of the story
        if (0==strcmp(currentScenario->name,"Ending.txt")){scenario_end_txt(currentScenario->name);break;}
        // option '1' is easier, '2' is harder, option '1' will return the integer 1, option '2' will return the integer 3
        int number_of_enemies = scenario_txt(currentScenario->name);// return a int type function
        Character enemies[number_of_enemies];
        printf("HERE\n");
        initialize_enemies_array(currentScenario->id, &enemies, number_of_enemies);
        // Check if battle is needed
        /*
        if (!currentScenario->battleWon) {
            if ("//win here//") {
                currentScenario->battleWon = true;
                printf("You won the battle in the %s!\n", currentScenario->name);
                // rewarding if he choose harder one
                if(number_of_enemies == 3){
                    currentState->character.atk += 50;
                    currentState->character.def += 50;
                    currentState->character.hp += 100;
                }
            } 
            else {
                printf("You lost the battle in the %s. Try again!\n", currentScenario->name);
                continue;
            }
        }*/
        scenario_end_txt(currentScenario->name);

        printf("\nYou are in the %s.\n", currentScenario->name);
        // Show adjacent scenarios
        showAdjacentScenarios(currentScenario);

        // Get user input for navigation
        printf("\nRecall memories or keep going:\n(Enter the ID of the next scenario you want to navigate to)\n");
        printf("\nEnter 0 to go to the menu:\n");
        int nextScenarioId = check_input(0,currentState->currentScenarioId+1);

        // change skill,go next scenario or before
        if (nextScenarioId == 0){
            run(currentState);
            /// Change skill function here //
            printf("\nYou are in the %s.\n", currentScenario->name);
            // Show adjacent scenarios
            showAdjacentScenarios(currentScenario);
            nextScenarioId = check_input(0,currentState->currentScenarioId+1);
        }
        else {currentState->currentScenarioId = nextScenarioId;}
        // Find the next scenario
        Scenario* nextScenario = getScenarioById(scenarios, nextScenarioId, MAX_SCENARIO);
        if (nextScenario == NULL) {
            printf("Invalid scenario ID. Try again.\n");
        } else {
            currentScenario = nextScenario;
        }
    }
}