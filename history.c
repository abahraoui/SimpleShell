#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "shell.h"

const int historySize = 20;
char *history[20][50]; /// array of separated strings
int historyCounter = 0; /// created a counter for vacant entries to store commands.
char *historyFileName = ".hist_list";

/**
 *
 * @param input a non-negative number
 * @return -1 if its an invalid input, otherwise, the number itself
 */
int getPosNumberFromString(char *input) {
    char *ptr;
    int result = strtol(input, &ptr, 10);
    if (result > 0) {
        if (result > historySize){
            printf("The history only holds %d commands.\n", historySize);
        }
        else if (result > historyCounter){
            printf("The array is currently only holding %d commands.\n", historyCounter);
        }
        else{
            return result;
        }
    } else {
            printf("History invocations only take numbers 1 - 20 as parameters.\n");
    }
    return -1;
}

void addToHistory(char *commandArray[50]) {
    ///trying to add all the input line into an entry of history.

    if (commandArray[0] != NULL && commandArray[0][0] == '!')
        return;
    if (historyCounter == historySize) {
        int k =0;
        while (history[0][k] != NULL) {
            history[0][k] = NULL;
            k++;
        }
        for (int j = 0; j < historySize - 1; j++) {
            int i = 0;
            while (history[j + 1][i] != NULL) {
                history[j][i] = malloc(sizeof(history[j + 1][i]));
                strcpy(history[j][i], history[j + 1][i]);
                history[j + 1][i] = NULL;
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
    if (historyCounter < historySize)
        historyCounter++;

    ///incrementing the counter for next vacant entry.
    /// keeping it circular by checking the counter vs size of array.
}


void printHistory() {
    int i = 0;
    char curCommand[512];
    while (history[i][0] != NULL && i < historySize) {
        strcpy(curCommand, "");
        int j = 0;
        while (history[i][j] != NULL){
        strcat(curCommand, history[i][j++]);
        strcat(curCommand, " ");
        }
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
    int print =0;
    char tempStr[100] = "";
    while (history[previousCommandIndex][print] != NULL){
        strcat(tempStr, history[previousCommandIndex][print]);
        strcat(tempStr, " ");
        print++;
    }
    printf("The history command you are executing is: %s \n", tempStr);
    readInput(history[previousCommandIndex]); /// executing the command
}

void executeNthCommandInt(int number) {
    int historyIndex = number - 1;
    /// checking if it's a right invoke.
    if ((history[historyIndex][0] == NULL) || historyIndex > (historyCounter - 1)) {
        printf("There is no command at position %d, try again.\n", number);
        return;
    }
    int print =0;
    char tempStr[100] = "";
    while (history[historyIndex][print] != NULL){
        strcat(tempStr, history[historyIndex][print]);
        strcat(tempStr, " ");
        print++;
    }
    printf("The history command you are executing is: %s \n", tempStr);
    readInput(history[historyIndex]);
}

void executeInverseNthCommandInt(int number) {
    const char *token;
    int historyIndex = historyCounter - number;
    /// looking for if the invocation is valid and can be executed.
    if ((history[historyIndex] == NULL) || historyIndex < 0) {
        printf("The history does not have %d records.\n", number);
        return;
    }
    int print =0;
    char tempStr[100] = "";
    while (history[historyIndex][print] != NULL){
        strcat(tempStr, history[historyIndex][print]);
        strcat(tempStr, " ");
        print++;
    }
    printf("The history command you are executing is: %s \n", tempStr);
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

void saveHistory() {
    chdir(savingPath);
    FILE *saveFile;
    if (access(historyFileName, F_OK) != 0) {
        printf("History file was not found or doesn't exist, creating a new one at %s/..\n", savingPath);
    }
    saveFile = fopen(historyFileName, "wb");
    for (int i = 0; i < historySize; i++) {
        if (*history[i] != NULL) {
            int j = 0;
            while (history[i][j] != NULL) {
                fputs(*(history[i] + j), saveFile);
                fputs(" ", saveFile);
                j++;
            }
            fputs("\n", saveFile);
        }
    }
    fclose(saveFile);
}


void loadHistory() {
    FILE *saveFile;
    if (access(historyFileName, F_OK) == 0) {
        saveFile = fopen(historyFileName, "r");
        int prev = ftell(saveFile);
        fseek(saveFile, 0, SEEK_END);
        int size = ftell(saveFile);
        fseek(saveFile, prev, SEEK_SET);

        if (size > 0) {
            char *buffer[50];
            char *ptr[51];
            char c;
            while ((c = fgetc(saveFile)) != EOF) {
                *buffer = NULL;
                fgets(buffer, 50, saveFile);
                ptr[0] = c;
                strcat(ptr, buffer);

                char *commandArray[50];
                const char *delimiter = " ;\t|><&\n";
                const char *token;
                /* get the first token */
                token = strtok(ptr, delimiter);
                /* walk through other tokens */
                int i = 0;
                while (token != NULL) {
                    commandArray[i] = token;
                    token = strtok(NULL, delimiter);
                    i++;
                }
                commandArray[i] = NULL;
                if (commandArray[0] == NULL)
                    continue;
                addToHistory(commandArray);
            }
            printf("loaded history correctly:\n");
            printHistory();
            fclose(saveFile);
        } else printf("%s\n", "File is empty");
    } else printf("History file not found or doesn't exist.\n");
}