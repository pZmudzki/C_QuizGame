#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "include/auth.h"

#define MAX_NICKNAME_LENGTH 11
#define MAX_PASSWORD_LENGTH 11

int auth(){
    return loginUser();
}


// function displaying login form to a user
int loginUser(){
    // variable keeping track if there has occured an error
    int error = 0;
    while(1){
        system("@cls||clear"); // initial console clear to prevent colliding with another locations

        char ch;

        if(error){
            printf("Wrong nickname or password, try again!\n\n");
        }

        printf("To login press any key\nTo register press ESC\n");
            // check if user presses esc key
            // if user presses esc return register function
            ch = getch();
            if((int)ch == 27){
                return registerUser();
            }

            system("@cls||clear"); // clear console for better visability

            char nickname[MAX_NICKNAME_LENGTH];
            char password[MAX_PASSWORD_LENGTH];

            printf("Nickname: ");
            scanf("%10s", &nickname);

            printf("Password: ");
            scanf("%10s", &password);

            int userID = checkCredentials(nickname, password);
            // if user passed wrong credentials
            // function checkCredentials will return 0;
            if(userID != 0){
                return userID;
            }
            // if user has entered wrong credentials, set an error to true
            error = 1;
    }
}

// function for displaying register form to a user
int registerUser(){

    while(1){
        system("@cls||clear"); // initial console clear to prevent colliding with another locations
        char ch;
        printf("To register press any key\nTo login press ESC\n");

        // check if user presses esc key
        // if user presses esc return login function
        ch = getch();
        if((int)ch == 27){
            return loginUser();
        }

        system("@cls||clear"); // clear console for better visability

        char nickname[MAX_NICKNAME_LENGTH];
        char password[MAX_PASSWORD_LENGTH];

        printf("Welcome, please register a new account.\n\n");

        printf("(max 10 characters)\nNickname: ");
        scanf("%10s", &nickname);

        printf("(max 10 characters)\nPassword: ");
        scanf("%10s", &password);

        //function saving new user to users.csv file
        saveNewUser(nickname, password);
        // check if saving worked
        return checkCredentials(nickname, password);
    }
}


// function checking if user passed correct nickname and password
int checkCredentials(char nickname[], char password[]){
    FILE *fp;
    int row_length = MAX_NICKNAME_LENGTH + MAX_PASSWORD_LENGTH + 10;
    char row[row_length];
    char *token;

    fp = fopen("./storage/users.csv","r");

    // row = user id, user nickname, user password
    while(!feof(fp)){ //feof(FILE *) - function detecting end of file
        fgets(row, row_length, fp);

        char foundNickname[MAX_NICKNAME_LENGTH];
        char foundPassword[MAX_PASSWORD_LENGTH];
        int id;

        // token - separated value from a row by a comma
        token = strtok(row, ",");

        // column - helper variable for identifying current read value in a row
        int column = 0;

        //iterate through a row till it's end
        while(token != NULL){
            // tokens
            // 0 - user id
            // 1 - nickname
            // 2 - password
            switch(column){
                case 0:
                    // copy string from a file to variable
                    strncpy(foundNickname, token, MAX_NICKNAME_LENGTH);
                    break;
                case 1:
                    strncpy(foundPassword, token, MAX_PASSWORD_LENGTH);
                    break;
                case 2:
                    id = atoi(token); // atoi() - function parsing string to an integer
                    break;
            }

            // set token to null to find rest of the tokens left in a row
            token = strtok(NULL, ",");

            // set the column to state that in next iteration function
            // should look for another type of value
            column++;
        }

        // check if imported nickname and password match the credentials user provided
        // if condition is true, function returns found user id
        // else returs 0 - no user found
        if(strcmp(foundNickname, nickname) == 0 && strcmp(foundPassword, password) == 0){
            return id;
        }
    }

    // close file stream
    fclose(fp);

    return 0;
}


// function saving new user to users.csv file
void saveNewUser(char nickname[], char password[]){
    FILE *fp;

    // variable for the last user id in a file
    int lastID = 0;
    int row_length = MAX_NICKNAME_LENGTH + MAX_PASSWORD_LENGTH + 10;
    char row[row_length];

    fp = fopen("./storage/users.csv","r");

    while(!feof(fp)){ //feof(FILE *) - function detecting end of file
        fgets(row, row_length, fp);
        // increase lastID till the end of a file
        lastID++;
    }

    //close stream
    fclose(fp);

    fp = fopen("./storage/users.csv", "a");

    // new user nickname, password, id
    fprintf(fp, "\n%s,%s,%d", nickname, password, lastID + 1);

    //close stream
    fclose(fp);

}






