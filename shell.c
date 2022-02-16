#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>

void execCommand(char *argv[]) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        execvp(argv[0], argv);
        perror("Error");
        exit(1);
    } else {
        wait(NULL);
    }
}

void getPathCommand() {
    printf("PATH: %s\n", getenv("PATH"));
}

void setPathCommand(char *newPath[]) {
    setenv("PATH", newPath[1], 1);
    printf("NEW PATH: %s\n", getenv("PATH"));
}

void run(void) {
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

        char *commandArray[50]; // Stage 2 change, String array which store commands.
        const char *delimiter = " ;\t|><&\n";
        const char *token;
        /* get the first token */
        token = strtok(d_buffer, delimiter);
        /* walk through other tokens */
        int i = 0;
        while (token != NULL) {
            //printf(" %s%s\n", "<", token);
            commandArray[i] = token;
            token = strtok(NULL, delimiter);
            i++;
        }
        commandArray[i] = NULL;

        if (commandArray[0] == NULL)
            continue;
        else if (strcmp(commandArray[0], "getpath") == 0)
            getPathCommand();
        else if (strcmp(commandArray[0], "setpath") == 0)
            setPathCommand(commandArray);
        else
            execCommand(commandArray);

    }
}
