#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

void execCommand(char *parameters[]) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        execvp(parameters[0], parameters);
        perror("Error");
        exit(1);
    } else {
        wait(NULL);
    }
}