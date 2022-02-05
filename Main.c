//
// Created by ykb20128 on 27/01/2022.
//
#include <stdio.h>
#include <stdbool.h>
#include "Main.h"
#include <string.h>
int main (void) {
    bool q = false;
    int c = 0;
    printf("%s\n","> Welcome to the Amogus Shell! Type exit or enter then CTRL+D to exit the shell");
    while ((q == false) && (c != EOF)){


        printf(">");

        char d_buffer[512];



        gets(d_buffer);
        if(d_buffer[0] == NULL){
            printf("%s\n",">Press enter again if you want to continue or type CTRL+D to exit the shell");
            c = fgetc(stdin);
        }
        /* if nothing was read from input, it then reads a new input waiting for CTRL+D, will put the value of
           CTRL+D(which is EOF) in c and it will break the while loop if c == EOF*/


        if (strcmp (d_buffer, "exit") == 0){
            q = true;
            break;
        }

        const char s[2] = " ";
        char *token;
        /* get the first token */
        token = strtok(d_buffer, s);
        /* walk through other tokens */
        while( token != NULL ) {
            printf( " %s%s\n", "<" ,token );

            token = strtok(NULL, s);
        }

    }
    return 0;
}
