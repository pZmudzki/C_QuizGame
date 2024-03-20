#include <stdio.h>
#include <conio.h>

#include "include/checkKey.h"

int menu(){
    system("@cls||clear"); // initial console clear to prevent colliding with another locations

    // currSelected - keeps the value of currently selected menu option
    int currSelected = 0;

    OutputMenuOptions(currSelected);
    while(1){
        // for each key press render new view
        // example:
        // if a user presses arrow key then this loop iterates over with updated view
        if (kbhit()) {
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
                    // return selected option
                    return currSelected;
            }
            system("@cls||clear"); // clean console
            OutputMenuOptions(currSelected);
        }
    }
}

// function displaying options to a user
void OutputMenuOptions(int selected){
    char menuOptions[3][12] = {"Start game", "Your stats", "Exit"};
    printf("Welcome, please select what you want to do:\n\n");
    for(int i = 0; i <= 2; i++){ // list options
        if(selected == i){
            printf("> %s\n", menuOptions[i]);
        } else {
            printf("  %s\n", menuOptions[i]);
        }
    }
}
