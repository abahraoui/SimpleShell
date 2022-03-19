#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "shell.h"

const int historySize = 20;
char *history[20][50]; /// array of separated strings
int historyCounter = 0; /// created a counter for vacant entries to store commands.

/**
 *
 * @param input a non-negative number
 * @return -1 if its an invalid input, otherwise, the number itself
 */
int getPosNumberFromString(char *input) {
    char *ptr;
    int result = strtol(input, &ptr, 10);
    if (result > 0) {
        if (result > historySize)
            printf("The history only holds %d commands.\n", historySize);
        else
            return result;
    } else {
        printf("Number %d is not allowed.\n", result);
    }
    return -1;
}

void addToHistory(char *commandArray[50]) {
    ///trying to add all the input line into an entry of history.

    if (commandArray[0] != NULL && commandArray[0][0] == '!')
        return;
    if (historyCounter == historySize){
        for(int j = 0; j < historySize-1; j++){
            int i = 0;
            while (history[j+1][i] != NULL) {
                history[j][i] = malloc(sizeof(history[j+1][i]));
                strcpy(history[j][i], history[j+1][i]);
                i++;
            }
        }
        historyCounter--;
    }
    int index = 0;
    while (commandArray[index] != NULL) {
        history[historyCounter][index] = malloc(sizeof(commandArray[index]));
        strcpy(history[historyCounter][index], commandArray[index]);
        index++;
    }
    if (historyCounter<historySize)
        historyCounter++;

    ///incrementing the counter for next vacant entry.
    /// keeping it circular by checking the counter vs size of array.
}


void printHistory() {
    int i = 0;
    char curCommand[512];
    while (history[i][0] != NULL) {
        strcpy(curCommand, "");
        int j = 0;
        while (history[i][j] != NULL)
            strcat(curCommand, history[i][j++]);
        printf("%d %s\n", i + 1, curCommand);
        i++;
    }
}

void executePreviousCommand() {
    int previousCommandIndex = (historyCounter + historySize - 1) % historySize;
    if (history[previousCommandIndex][0] == NULL) {   /// looking for if the invocation is valid and can be executed.
        printf("%s\n", "There is no previous command.");
        return;
    }
    readInput(history[previousCommandIndex]); /// executing the command
}

void executeNthCommandInt(int number) {
    int historyIndex = number - 1;
    /// checking if it's a right invoke.
    if ((history[historyIndex][0] == NULL) || historyIndex > (historyCounter - 1)) {
        printf("There is no command at position %d, try again.\n", number);
        return;
    }
    readInput(history[historyIndex]);
}

void executeInverseNthCommandInt(int number) {
    const char *token;
    int historyIndex = historyCounter - number;
    /// looking for if the invocation is valid and can be executed.
    if ((history[historyIndex] == NULL) || historyIndex<0) {
        printf("The history does not have %d records.\n", number);
        return;
    }
    readInput(history[historyIndex]);

}


void executeHistoryInvocation(char *commandArray[50]) {
    if (commandArray[1] != NULL) {
        printf("History invocation does not allow any parameters.\n");
        return;
    }
    if (strcmp(commandArray[0], "!!") == 0) // !!
        executePreviousCommand();
    else if (commandArray[0][0] == '!' && commandArray[0][1] == '-') {// !-<no> !-2 --> "2" --> 2 --> -2
        int inputNumber = getPosNumberFromString(commandArray[0] + 2);
        if (inputNumber == -1) return;
        executeInverseNthCommandInt(inputNumber);
    } else {
        int inputNumber = getPosNumberFromString(commandArray[0] + 1);
        if (inputNumber == -1) return;
        executeNthCommandInt(inputNumber);
    }
}

void saveHistory(){
    chdir("/home/gbb20158/CLionProjects/Shell9");
    FILE *saveFile;
    saveFile = fopen("history.hist_list", "wb");

    for(int i = 0; i<historySize; i++){
        if(*history[i] != NULL){
            int j = 0;
         while(history[i][j] != NULL){
             fputs(*(history[i] + j),saveFile);
             fputs(" ", saveFile);
             j++;
         }

         fputs("\n", saveFile);


        }



    }



    fclose(saveFile);
}

void loadHistory(){
    chdir("/home/gbb20158/CLionProjects/Shell9");
    FILE *saveFile;
    saveFile = fopen("history.hist_list", "rb");
    char buffer[50];
    char buffer2[50];
  //  while(fscanf(saveFile,"%s", buffer)!= EOF) fscanf(saveFile,"%s", buffer);
    fgets(buffer, 50, saveFile);
    fgets(buffer2, 50, saveFile);
    printf("%s %s\n",buffer, buffer2 );


   /* for(int i = 0; i<historySize; i++){
        if(fscanf(saveFile, "%[^\n]", history[i]) > 0) fscanf(saveFile, "%[^\n]", history[i]);


    }*/

}