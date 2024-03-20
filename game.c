#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>


#include "include/checkKey.h"

#define MAX_ROUNDS 10

struct question{  // structure for question
    char question[100];
    char answers[4][50];
    int rightAnswer; // idx of answers arr
};
typedef struct question Question;

void game(int userID){
    system("@cls||clear"); // initial console clear to prevent colliding with another locations

    // random integer arr which implies questions id
    int generatedQuestionsIds[MAX_ROUNDS];
                //      generated ids arr, question count, max idx
    generateRandomIds(generatedQuestionsIds, MAX_ROUNDS, 49);


    Question questions[11];
    getQuestionsFromStorage(generatedQuestionsIds, MAX_ROUNDS, questions);


    // round - variable keeping track of a round, round 0 is the first round
    // rightAns, wrongAns - variables keeping track of bad and good answers
    // gameStarted - 1 = game is on, 0 = game has ended
    // currSelected - keeps the value of currently selected answer
    int round, currSelected, rightAns, wrongAns, gameStarted;

    round = 0;
    rightAns = wrongAns = 0;
    gameStarted = 1;
    currSelected = 0;


    printf("Question %d/%d\n\n", round + 1, MAX_ROUNDS);
    OutputOptions(currSelected, questions[round]); // output first question

    while (gameStarted == 1) {
        if (kbhit()) { // for each key press render new view
            int pressed_key = checkKey(); // check what key has been pressed
            switch(pressed_key){
                case 72: // ascii code for up key
                    if(currSelected > 0){
                        currSelected--;
                    }
                    break;
                case 80: // ascii code for down key
                    if(currSelected < 3){
                        currSelected++;
                    }
                    break;
                case 13: // ascii code for enter key
                    if(questions[round].rightAnswer == currSelected){
                        rightAns++;
                    } else {
                        wrongAns++;
                    }
                    round++;
                    break;
            }
            system("@cls||clear"); // clean console
            if(round == MAX_ROUNDS){
                gameStarted = 0;
            } else {
                printf("Question %d/%d\n\n", round + 1, MAX_ROUNDS);
                OutputOptions(currSelected, questions[round]); // if game is on display options after each key press
            }
        }
    }
    /*// output ending screen with an option to restart
    if(OutputEnd(rightAns, wrongAns) == 1){
        round = 0;
        rightAns = wrongAns = 0;
        gameStarted = 1;
        system("@cls||clear");
        goto start;
    } */

    saveGame(rightAns, wrongAns, userID);
    OutputEnd(rightAns, wrongAns);
}


//function for displaying options and add > to option that user is pointing to
void OutputOptions(int selected, Question q){
    char options[] = "abcd";
    printf("%s\n", q.question);
    for(int i = 0; i <= 3; i++){ // list options
        if(selected == i){
            printf("> %c. %s\n", options[i], q.answers[i]);
        } else {
            printf("  %c. %s\n", options[i], q.answers[i]);
        }
    }
}

// function displaying endgame screen with an option to restart it
void OutputEnd(int rigth, int wrong){
    printf("Rigth answers: %d\nWrong answers: %d\n\nPress ANY key to go back to Menu\n", rigth, wrong);
    while(1){
        if(kbhit()){
            break;
        }
    }
}

int generateRandomIds(int random_arr[], int arr_length, int max_num){
    // Seed the random number generator with the current time
    srand(time(NULL));

    int count = 0; // Variable to keep track of the number of generated numbers

    while (count < arr_length) {
        int random_number = (rand() % max_num) + 1; // Generate random number between 1 and 50

        // Check if the random number is already in the array
        int is_duplicate = 0;
        for (int i = 0; i < count; i++) {
            if (random_arr[i] == random_number) {
                is_duplicate = 1;
                break;
            }
        }

        // If the random number is not a duplicate, add it to the array
        if (!is_duplicate) {
            random_arr[count] = random_number;
            count++;
        }
    }
    return 0;
}


// import questions from csv file based on generated ids
int getQuestionsFromStorage(int randArr[], int randArrLength, Question questions[]){
    FILE *fp;
    char row[350];
    char *token;

    int currRow = 1;
    int currIdx = 0;

    fp = fopen("./storage/questions.csv","r");

    // row = id,question,answer1,answer2,answer3,answer4,rightAns
    while (!feof(fp)){
        fgets(row, 350, fp);

        for(int i = 0; i <= randArrLength - 1; i++){

            if(randArr[i] == currRow){

                token = strtok(row, ",");

                char question[100];
                char answers[4][50];
                int rightAnswer;

                int column = 0;
                while(token != NULL){

                    switch(column){
                        case 1:
                            strncpy(question, token, 100);
                            break;
                        case 2:
                            strncpy(answers[0], token, 50);
                            break;
                        case 3:
                            strncpy(answers[1], token, 50);
                            break;
                        case 4:
                            strncpy(answers[2], token, 50);
                            break;
                        case 5:
                            strncpy(answers[3], token, 50);
                            break;
                        case 6:
                            rightAnswer = atoi(token);
                            break;
                    }
                    token = strtok(NULL, ",");
                    column++;
                }
                Question newQuestion; // init a question "class" helper

                strncpy(newQuestion.question, question, 100); // copy question str to newQuestion

                // copy answers to newQuestion answers
                for(int k = 0; k <= 3; k++){
                    strncpy(newQuestion.answers[k], answers[k], 50);
                }
                // copy right answer to newQuestion
                newQuestion.rightAnswer = rightAnswer;
                questions[currIdx] = newQuestion;
                currIdx++;
            }
        }
        currRow++;
    }
    fclose(fp);
    return 0;
}

int saveGame(int right, int wrong, int userID){
    int lastID = 0;
    char row[100];

    FILE *fp;
    fp = fopen("./storage/games.csv","r");

    while(!feof(fp)){
        fgets(row, 100, fp);
        lastID++;
    }

    fclose(fp);

    fp = fopen("./storage/games.csv","a");

    // game id, user id, ans right, ans wrong
    fprintf(fp, "\n%d,%d,%d,%d", lastID + 1, userID, right, wrong);

    fclose(fp);

    return 0;
}




