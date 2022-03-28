#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "history.h"
#include "shell.h"
#include "aliases.h"
#include <wait.h>

///this is to make sure the program does not go into an infinite loop - stage 9 - has to be defined outside of the function
int numberOfTimes = 0;

void changeDirectoryCommand(char *parameters[]) {
    if (parameters[1] == NULL) {
        chdir(getenv("HOME"));
    } else if (parameters[2] != NULL) {
        printf(" < CD should have at most 1 parameter, but I received more, please try removing the other parameters.\n");
    } else {
        int success = chdir(parameters[1]);
        char errorMessage[512] = "Error accessing ";
        strcat(errorMessage, parameters[1]);
        if (success == -1) perror(errorMessage);
    }
}

void getPathCommand(char *parameters[]) {
    if (parameters[1] == NULL)
        printf(" < PATH: %s\n", getenv("PATH"));
    else
        printf(" < GETPATH should not have any parameter!\n");
}

void setPathCommand(char *parameters[]) {
    if (parameters[1] == NULL)
        printf(" < SETPATH should have 1 string parameter, but I received none.\n");
    else if (parameters[2] != NULL)
        printf(" < SETPATH should have only 1 parameter, but I received more, please try removing the other parameters.\n");
    else {
        setenv("PATH", parameters[1], 1);
        printf(" < NEW PATH: %s\n", getenv("PATH"));
    }
}

void readInput(char *commandArray[]) {
    numberOfTimes++;
    if (numberOfTimes > 10){
        printf("You either have entered an infinite loop or tried to chain more than 10 commands, halting current chain of commands!\n");
        numberOfTimes=0;
        return;
    }
    if (tryToRunAliasCommand(commandArray) == 1)
       return;
    else if (strcmp(commandArray[0], "getpath") == 0)
        getPathCommand(commandArray);
    else if (strcmp(commandArray[0], "setpath") == 0)
        setPathCommand(commandArray);
    else if (strcmp(commandArray[0], "cd") == 0)
        changeDirectoryCommand(commandArray);
    else if (strcmp(commandArray[0], "history") == 0) {
        if (commandArray[1] != NULL) {
            printf("The history command does not allow any parameters.\n");
            return;
        }
        printHistory();
    } else if (commandArray[0][0] == '!')
        executeHistoryInvocation(commandArray);
    else if (strcmp(commandArray[0], "alias") == 0)
        addToAlias(commandArray);
    else if (strcmp(commandArray[0], "unalias") == 0)
        removeAlias(commandArray);
    else {
        execCommand(commandArray);
    }
    numberOfTimes = 0;
}


void run(void) {
    chdir(getenv("HOME"));
    getcwd(savingPath, sizeof(savingPath));
    loadHistory();
    loadAliases();
    char *currentPath[PATH_MAX];
    while (1) {
        getcwd(currentPath, sizeof(currentPath));
        printf("%s> ", currentPath);
        char *d_buffer[512];
        if (fgets(d_buffer, 512, stdin) == NULL)
            break;
        if (strcmp(d_buffer, "exit\n") == 0)
            break;
        char *commandArray[50];
        const char *delimiter = " ;\t|><&\n";
        const char *token;
        /* get the first token */
        token = strtok(d_buffer, delimiter);
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
        readInput(commandArray);   ///put all the reading input to choose command in a helper function.
    }
    saveHistory();
    saveAliases();
}
void execCommand(char *parameters[]) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        execvp(parameters[0], parameters);
        char errorMessage[512] = "Error trying to execute ";
        strcat(errorMessage, parameters[0]);
        perror(errorMessage);
        exit(1);
    } else {
        wait(NULL);
    }
}