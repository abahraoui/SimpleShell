#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "shell.h"

const int HISTORY_SIZE = 20;
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
        if (result > HISTORY_SIZE)
            printf("The history only holds %d commands.\n", HISTORY_SIZE);
        else
            return result;
    } else {
        printf("Number %d is not allowed.\n", result);
    }
    return -1;
}

void addToHistory(char *commandArray[50]) {
    ///trying to add all the input line into an entry of history.
    int i = 0;
    if (commandArray[0] != NULL && commandArray[0][0] == '!')
        return;
    while (commandArray[i] != NULL) {
        history[historyCounter][i] = malloc(sizeof(commandArray[i]));
        strcpy(history[historyCounter][i], commandArray[i]);
        i++;
    }
    ///incrementing the counter for next vacant entry.
    /// keeping it circular by checking the counter vs size of array.
    if (historyCounter == sizeof(history) / sizeof(history[0]) - 1)
        historyCounter = 0; ///kinda sus to me, idk if it works
    else historyCounter++;
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
    int previousCommandIndex = (historyCounter + HISTORY_SIZE - 1) % HISTORY_SIZE;
    if (history[previousCommandIndex][0] == NULL) {   /// looking for if the invocation is valid and can be executed.
        printf("%s\n", "There is no previous command.");
        return;
    }
    readInput(history[previousCommandIndex]); /// executing the command
}

void executeNthCommand(char *commandArray[50]) {
    int historyIndex;
    char *ptr;
    const char *delimiter = "!";   /// getting right index
    commandArray[0] = strtok(commandArray[0], delimiter);
    historyIndex = strtol(commandArray[0], ptr, 10);
    historyIndex -= 1;
    /// checking if it's a right invoke.
    if ((history[historyIndex][0] == NULL) || (historyIndex == historyCounter) || historyIndex < 0 ||
        historyIndex >= (historyCounter - 1)) {
        printf("%s\n", "Invocation invalid, try again.");
        return;
    }

    readInput(history[historyIndex]);
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

void executeInverseNthCommand(char *commandArray[50]) {
    const char *token;
    int historyIndex;
    const char *delimiter = "!-";
    commandArray[0] = strtok(commandArray[0], delimiter); /// parsing to get the right index
    historyIndex = atoi(commandArray[0]);
    historyIndex = historyCounter - historyIndex;
    /// looking for if the invocation is valid and can be executed.
    if ((history[historyIndex] == NULL) || (historyIndex == historyCounter)) {
        printf("%s\n", "Invocation invalid, try again.");
        return;
    }
    /// looking for whether it's a valid index to invoke.
    if ((historyIndex >= 0) && (historyIndex < 20)) {
        if (strcmp(history[historyIndex], "history") != 0) {
            token = strtok(history[historyIndex], " ");
            char *commandInvoked[50];
            int i = 0;
            while (token != NULL) {  ///usual parsing, dont like multiple parameter strings.
                commandInvoked[i] = token;

                token = strtok(NULL, " ");
                i++;
            }
            readInput(commandInvoked);
        } else {
            int i = 0; /// usual printing if it's asked for.
            while (history[i] != NULL) {
                printf("%d %s\n", i + 1, *(history + i));
                i++;
            }
        }
    }
}

void executeInverseNthCommandInt(int number) {
    const char *token;
    int historyIndex = historyCounter - number;
    /// looking for if the invocation is valid and can be executed.
    if ((history[historyIndex] == NULL)) {
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