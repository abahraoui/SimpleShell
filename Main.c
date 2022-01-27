//
// Created by ykb20128 on 27/01/2022.
//
#include <stdio.h>
#include <stdbool.h>
#include "Main.h"
#include <string.h>
int main (void) {
    bool q = false;
    char errormsg[] = "Wrong input, either put in 0-9 or q to quit.";
    while (q == false){
        char d_buffer[513];
        printf(">");
        gets(d_buffer);
        if (strcmp (d_buffer, "exit") ==0){
            q = true;
            break;
        }

        const char s[2] = " ";
        char *token;
        /* get the first token */
        token = strtok(d_buffer, s);
        /* walk through other tokens */
        while( token != NULL ) {
            printf( " %s\n", token );

            token = strtok(NULL, s);
        }

    }
    return 0;
}