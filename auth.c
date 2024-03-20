#include <stdio.h>
#include <string.h>
#include <conio.h>

#include "include/auth.h"

#define MAX_NICKNAME_LENGTH 11
#define MAX_PASSWORD_LENGTH 11

int auth(){
    return loginUser();
}


int loginUser(){
    int error = 0;
    while(1){
        system("@cls||clear");
        char ch;

        if(error){
            printf("Wrong nickname or password, try again!\n\n");
        }

        printf("To login press any key\nTo register press ESC\n");
            // check if user presses esc key
            ch = getch();
            if((int)ch == 27){
                return registerUser();
            }

            system("@cls||clear");

            char nickname[MAX_NICKNAME_LENGTH];
            char password[MAX_PASSWORD_LENGTH];

            printf("Nickname: ");
            scanf("%10s", &nickname);

            printf("Password: ");
            scanf("%10s", &password);

            int userID = checkCredentials(nickname, password);
            if(userID != 0){
                return userID;
            }
            error = 1;
    }
}


int registerUser(){

    while(1){
        system("@cls||clear");
        char ch;
        printf("To register press any key\nTo login press ESC\n");

        // check if user presses esc key
        ch = getch();
        if((int)ch == 27){
            return loginUser();
        }

        system("@cls||clear");

        char nickname[MAX_NICKNAME_LENGTH];
        char password[MAX_PASSWORD_LENGTH];

        printf("Welcome, please register a new account.\n\n");

        printf("(max 10 characters)\nNickname: ");
        scanf("%10s", &nickname);

        printf("(max 10 characters)\nPassword: ");
        scanf("%10s", &password);

        saveNewUser(nickname, password);
        return checkCredentials(nickname, password);
    }
}


int checkCredentials(char nickname[], char password[]){
    FILE *fp;
    int row_length = MAX_NICKNAME_LENGTH + MAX_PASSWORD_LENGTH + 10;
    char row[row_length];
    char *token;

    fp = fopen("./storage/users.csv","r");

    while(!feof(fp)){
        fgets(row, row_length, fp);

        char foundNickname[MAX_NICKNAME_LENGTH];
        char foundPassword[MAX_PASSWORD_LENGTH];
        int id;

        token = strtok(row, ",");
        int column = 0;
        while(token != NULL){
            switch(column){
                case 0:
                    strncpy(foundNickname, token, MAX_NICKNAME_LENGTH);
                    break;
                case 1:
                    strncpy(foundPassword, token, MAX_PASSWORD_LENGTH);
                    break;
                case 2:
                    id = atoi(token);
                    break;
            }

            token = strtok(NULL, ",");
            column++;
        }

        if(strcmp(foundNickname, nickname) == 0 && strcmp(foundPassword, password) == 0){
            return id;
        }
    }
    fclose(fp);

    return 0;
}

void saveNewUser(char nickname[], char password[]){
    FILE *fp;
    int lastID = 0;
    int row_length = MAX_NICKNAME_LENGTH + MAX_PASSWORD_LENGTH + 10;
    char row[row_length];

    fp = fopen("./storage/users.csv","r");

    while(!feof(fp)){
        fgets(row, row_length, fp);
        lastID++;
    }

    fclose(fp);

    fp = fopen("./storage/users.csv", "a");

    fprintf(fp, "\n%s,%s,%d", nickname, password, lastID + 1);

    fclose(fp);

}






