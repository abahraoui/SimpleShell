#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <ctype.h>

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

char *trimString(char *str) {
    size_t len = strlen(str);

    while (isspace(str[len - 1])) --len;
    while (*str && isspace(*str)) ++str, --len;

    return strndup(str, len);
}