#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "shell.h"

void run(){
printf("%s\n", "> Welcome to the Amogus Shell! Type exit or CTRL+D to exit the shell");
    while (1) {
        char d_buffer[512];
        char *inputArray[30];
        int i = 0;
        printf("> ");

        if (fgets(d_buffer, 512, stdin) == NULL)
            break;
        if (strcmp(d_buffer, "exit\n") == 0)
            break;

      const char *s = " ;\t|><&;";
      char *token;
      /* get the first token */
        token = strtok(d_buffer, s);
      /* walk through other tokens */
        while (token != NULL) {
            printf(" %s%s\n", "<", token);
            inputArray[i] = token;
            token = strtok(NULL, s);
            i++;
        }
        execFork(inputArray);
    }
    printf("\nExiting...\n");
}

void execFork(char *inputArr[]){
    int result;
    int i = 0;

    int pid = fork();

    if(pid < 0){
        printf("Fork was unsuccessful.\n");
    } else if(pid == 0){
        result = execvp(inputArr[0], inputArr);

        if(result == -1){
            char error[200];
            snprintf(error, sizeof(error), "Erroe on %s", inputArr[0]);
            perror(error);
            exit(1);
        }
    } else {
        wait(NULL);
    }
}