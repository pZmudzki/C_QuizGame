#include <stdio.h>
#include <string.h>

#include "include/game.h"
#include "include/menu.h"
#include "include/auth.h"

int main() {
    int user; // user id
    // auth returns user id
    user = auth();
    while(1){
        // menu returns:
        // 0 if user picked to start a game
        // 1 if user picked to view stats
        // 2 if user picked to exit an app
        switch(menu()){
            case 0:
                game(user);
                break;
            case 1:
                statistics(user);
                break;
            case 2:
                printf("\nThanks for playing, see ya!\n");
                return 0;
        }
    }
}
