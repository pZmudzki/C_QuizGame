#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>


#include "include/checkKey.h"

#define MAX_ROUNDS 10
#define QUESTIONS_COUNT 49

// structure for question
struct question{
    char question[100];
    char answers[4][50];
    int rightAnswer; // idx of answers arr
};
// type definition for question struct
typedef struct question Question;


// function responsible for tracking and displaying quiz to a user
void game(int userID){
    system("@cls||clear"); // initial console clear to prevent colliding with another locations

    // random integer arr which implies questions id in the questions.csv file
    int generatedQuestionsIds[MAX_ROUNDS];

    // function generating random ids for passed array arr
    //args: array, question count, max possible id
    generateRandomIds(generatedQuestionsIds, MAX_ROUNDS, QUESTIONS_COUNT);


    // array for storing questions
    Question questions[MAX_ROUNDS + 1];

    //function that gets every question from questions.csv
    // and stores them in question array
    // args: generated ids array, how many questions to get, an array to store questions
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
                    if(currSelected < 3){
                        currSelected++;
                    }
                    break;
                case 13: // ascii code for enter key
                    // if user selected the correct answer increase rightAns
                    // otherwise increase wrongAns
                    if(questions[round].rightAnswer == currSelected){
                        rightAns++;
                    } else {
                        wrongAns++;
                    }
                    round++;
                    break;
            }
            system("@cls||clear"); // clean console
            // end a game and display game stats if user reaches max rounds
            // otherwise render a question
            if(round == MAX_ROUNDS){
                gameStarted = 0;
            } else {
                printf("Question %d/%d\n\n", round + 1, MAX_ROUNDS);
                OutputOptions(currSelected, questions[round]); // if game is on, display options after each key press
            }
        }
    }

    // function saving a game to games.csv file
    saveGame(rightAns, wrongAns, userID);
    // function displaying end results
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

// function displaying endgame screen
void OutputEnd(int rigth, int wrong){
    printf("Rigth answers: %d\nWrong answers: %d\n\nPress ANY key to go back to Menu\n", rigth, wrong);
    while(1){
        // wait for user to press any key to stop the loop and exit a function
        if(kbhit()){
            break;
        }
    }
}

// function generating random ids for passed array arr
//args: array, question count, max possible id
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


// import questions from questions.csv file based on generated ids
int getQuestionsFromStorage(int randArr[], int randArrLength, Question questions[]){
    FILE *fp;
    char row[350];
    char *token;

    // helper variable keeping track of a question id
    // (ids in a file are sorted and start with 1)
    int currRow = 1;

    // keeps track of questions array id to
    // set imported question to right index
    int currIdx = 0;

    fp = fopen("./storage/questions.csv","r");

    // row = id,question,answer1,answer2,answer3,answer4,rightAns
    while (!feof(fp)){ //feof(FILE *) - function detecting end of file
        fgets(row, 350, fp);

        // iterate till i reaches random generated array length
        for(int i = 0; i <= randArrLength - 1; i++){

            if(randArr[i] == currRow){

                // token - separated value from a row by a comma
                token = strtok(row, ",");

                char question[100];
                char answers[4][50];
                int rightAnswer;

                // column - helper variable for identifying current read value in a row
                int column = 0;

                while(token != NULL){
                    // tokens
                    // 0 - id of a question
                    // 1 - question
                    // 2 - answer 1
                    // 3 - answer 2
                    // 4 - answer 3
                    // 5 - answer 4
                    // 6 - correct answer id
                    switch(column){
                        case 1:
                            strncpy(question, token, 100); // copy string from a file to variable
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

                    // set token to null to find rest of the tokens left in a row
                    token = strtok(NULL, ",");
                    // set the column to state that in next iteration function
                    // should look for another type of value
                    column++;
                }
                Question newQuestion; // init a question struct helper

                strncpy(newQuestion.question, question, 100); // copy question str to newQuestion

                // copy answers to newQuestion answers
                for(int k = 0; k <= 3; k++){
                    strncpy(newQuestion.answers[k], answers[k], 50);
                }
                // copy right answer to newQuestion
                newQuestion.rightAnswer = rightAnswer;

                // set question arr value at currIdx to imported question
                questions[currIdx] = newQuestion;
                //increase index
                currIdx++;
            }
        }

        currRow++;
    }
    //close stream
    fclose(fp);
    return 0;
}

// function saving a game to games.csv file
int saveGame(int right, int wrong, int userID){
    // variable for the last id of games in a file
    int lastID = 0;
    char row[100];

    FILE *fp;
    fp = fopen("./storage/games.csv","r");

    while(!feof(fp)){ //feof(FILE *) - function detecting end of file
        fgets(row, 100, fp);
        // increase lastID till the end of a file
        lastID++;
    }

    //close stream
    fclose(fp);

    fp = fopen("./storage/games.csv","a");

    // game id, user id, ans right, ans wrong
    fprintf(fp, "\n%d,%d,%d,%d", lastID + 1, userID, right, wrong);

    //close stream
    fclose(fp);

    return 0;
}




