#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>

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

void changeDirectoryCommand(char *parameters[]){
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

void printHistory(char * history){
    int i = 0;
    while(history[i] != NULL){
        printf("%s\n", *(history + i));
        i++;
    }
}

void readInput(char *commandArray[]){
    if (strcmp(commandArray[0], "getpath") == 0)
        getPathCommand(commandArray);
    else if (strcmp(commandArray[0], "setpath") == 0)
        setPathCommand(commandArray);
    else if (strcmp(commandArray[0], "cd") == 0)
        changeDirectoryCommand(commandArray);
    else
        execCommand(commandArray);
}

void run(void) {
    chdir(getenv("HOME"));
    char *currentPath[PATH_MAX];
    char *history[20]; // created a history string array.

    int historyCounter = 0; // created a counter for vacant entries to store commands.
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

        //trying to add all the input line into an entry of history.
        history[historyCounter] = malloc(sizeof(commandArray)*sizeof(commandArray[0]));
        int j = 0;
        while( commandArray[j] != NULL){
            if(strcspn(commandArray[j], "!") == 0) break;
            if(strcspn(commandArray[j], "!") != 0)strcat(history[historyCounter], commandArray[j]);
            if(commandArray[j+1] != NULL) strcat(history[historyCounter], " ");
            j++;
        }


        if(historyCounter == sizeof(history)/sizeof(history[0]) - 1) historyCounter = 0;
       else historyCounter++; //incrementing the counter for next vacant entry.



        if (commandArray[0] == NULL) continue;
        else if(strcmp(commandArray[0], "history") == 0){   // printing history
            int i = 0;
            while(history[i] != NULL){
                printf("%s\n", *(history + i));
                i++;
            }

        }
        // here is the issue with !<no>
        else if(strcspn(commandArray[0], "!") == 0){
            int historyIndex;
            // char charIndex = ? command array[0] = !2 -> 2
            //  printf("%c", charIndex);
           //   historyIndex = charIndex - '0';
            printf("%d", historyIndex);
            token = strtok(history[historyIndex], " ");
           char * commandInvoked[50];
           int i = 0;
           while(token != NULL){
               commandInvoked[i] = token;
               token = strtok(NULL, " ");
               i++;
           } // post processing working just missing the right index.
           int j = 0;
           while(commandInvoked[j] != NULL){
               printf("%s\n", *(history + j));
               j++;
           }
            execCommand(commandInvoked);
        }

        else readInput(commandArray);   //put all the reading input to choose command in a helper function.

    }


}
