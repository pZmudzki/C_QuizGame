#include <stdio.h>
#include <conio.h>

#include "include/checkKey.h"

int menu(){
    system("@cls||clear"); // initial console clear to prevent colliding with another locations

    int currSelected = 0;

    OutputMenuOptions(currSelected);
    while(1){
        if (kbhit()) { // for each key press render new view
            int pressed_key = checkKey(); // check what key has been pressed
            switch(pressed_key){
                case 72: // ascii code for up key
                    if(currSelected > 0){
                        currSelected--;
                    }
                    break;
                case 80: // ascii code for down key
                    if(currSelected < 2){
                        currSelected++;
                    }
                    break;
                case 13: // ascii code for enter key
                    return currSelected;
            }
            system("@cls||clear"); // clean console
            OutputMenuOptions(currSelected);
        }
    }
}

void OutputMenuOptions(int selected){
    char menuOptions[3][12] = {"Start game", "Your stats", "Exit"};
    printf("Welcome, please select what you want to do:\n\n");
    //printf("%s\n\n", menuOptions[]);
    for(int i = 0; i <= 2; i++){ // list options
        if(selected == i){
            printf("> %s\n", menuOptions[i]);
        } else {
            printf("  %s\n", menuOptions[i]);
        }
    }
}
