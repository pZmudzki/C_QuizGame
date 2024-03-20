#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


#include "include/statistics.h"

// function displaying user stats: games played, right answers procentage, wrong answers procentage
// function doesn't need to return a value and that's why it is void
void statistics(int userID){
    system("@cls||clear"); // initial console clear to prevent colliding with another locations

    int gamesCount = 0, rightAnswerCount = 0, wrongAnswerCount = 0;

    // function that gets every record that has current logged user from games.csv file
    // and updates passed variables directly inside
    getStatsFromStorage(userID, &gamesCount, &rightAnswerCount, &wrongAnswerCount);

    // formatting rigth/wrong answers count to a precentage
    float rightAnswerRatio = (gamesCount > 0) ? (((float)rightAnswerCount / ((float)gamesCount * 10)) * 100) : 0.0;
    float wrongAnswerRatio = (gamesCount > 0) ? (((float)wrongAnswerCount / ((float)gamesCount * 10)) * 100) : 0.0;

    printf("Games played: %d\nRight answers: %.1f %%\nWrong answers: %.1f %%\n\nPress ANY key to go back to Menu\n", gamesCount, rightAnswerRatio, wrongAnswerRatio);


    while(1){
        // wait for user to press any key to stop the loop and execute a function
        if(kbhit()){
            break;
        }
    }
}


// function that gets every record that has current logged user from games.csv file
// and updates passed variables directly inside
int getStatsFromStorage(int userID, int *gamesCount, int *rightAnswerCount, int *wrongAnswerCount){
    FILE *fp;
    char row[10];
    char *token;

    fp = fopen("./storage/games.csv","r");

    // row = game id, user id, right ans, wrong ans
    while(!feof(fp)){ //feof(FILE *) - function detecting end of file
        fgets(row, 10, fp);

        // token - separated value from a row by a comma
        token = strtok(row, ",");

        // curr row, row values
        int rightAns, wrongAns, user;
        int column = 0;

        //iterate through a row till it's end
        while(token != NULL){
            // column - helper variable for identifying current read value in a row
            // 0 - game id
            // 1 - user id
            // 2 - rigth answers count
            // 3 - wrong answers count
            switch(column){
                case 1:
                    user = atoi(token); // atoi() - function parsing string to an integer
                    break;
                case 2:
                    rightAns = atoi(token);
                    break;
                case 3:
                    wrongAns = atoi(token);
                    break;
            }

            // set token to null to find rest of the tokens left in a row
            token = strtok(NULL, ",");
            // set the column to state that in next iteration function
            // should look for another type of value
            column++;
        }

        // if user id in a row is equal to current logged in user id
        // function will update passed in args
        if(user == userID){
            *gamesCount += 1;
            *rightAnswerCount += rightAns;
            *wrongAnswerCount += wrongAns;
        }
    }

    // close file stream
    fclose(fp);
    return 0;
}
