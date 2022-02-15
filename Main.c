//
// Created by ykb20128 on 27/01/2022.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>



int main(void) {

    printf("%s\n", "> Welcome to the Amogus Shell! Type exit or CTRL+D to exit the shell");

    while (1) {
        int i = 0;
        printf("> ");

       const char *d_buffer[512];

        if (fgets(d_buffer, 512, stdin) == NULL)
            break;
        if (strcmp(d_buffer, "exit\n") == 0)
            break;

      char *commands[50]; // Stage 2 change, String array which store commands.



        const char *s = " ;\t|><&\n;";
        const char *token;
        /* get the first token */
        token = strtok(d_buffer, s);
        /* walk through other tokens */
        while (token != NULL) {
            printf(" %s%s\n", "<", token);
            commands[i] = token;
            token = strtok(NULL, s);
            i++;
        }

         pid_t pid = fork();
         if(pid < 0){
             perror("fork failed");
             exit(1);
         }
         else if(pid == 0){
             execvp(commands[0], commands);
         }
         wait(NULL);

    }

    printf("\nExiting...\n");
    return 0;
}
