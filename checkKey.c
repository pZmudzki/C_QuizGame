#include <stdio.h>
#include <conio.h>

#include "include/checkKey.h"

//function returning key code that has been pressed
int checkKey() {
    int key;
    if (kbhit()) {
        key=getch();
        if (key == 224) {
            do {
                key=getch();
            } while(key==224);

            return key;
        }
    }
}
