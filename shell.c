#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "history.h"
#include "shell.h"
#include "aliases.h"


void changeDirectoryCommand(char *parameters[]) {
    if (parameters[2] != NULL) {
        printf(" < CD should have at most 1 parameter, but I received more, please try removing the other parameters.\n");
    } else if (parameters[1] == NULL) {
        chdir(getenv("HOME"));
    } else if (parameters != NULL) {
        int success = chdir(parameters[1]);
        if (success == -1) perror("Error");

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
    if (strcmp(commandArray[0], "getpath") == 0)
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
    else
        execCommand(commandArray);
}


void run(void) {
    getcwd(savingPath, sizeof(savingPath));
    loadHistory();
    loadAliases();
    chdir(getenv("HOME"));
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

        if (commandArray[0] == NULL) continue;

        addToHistory(commandArray);

        readInput(commandArray);   ///put all the reading input to choose command in a helper function.
    }
    saveHistory();
    saveAliases();
}
