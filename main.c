//
// Created by ykb20128 on 27/01/2022.
//
#include <stdio.h>
#include "shell.h"

int main(void) {
    printf("%s\n", "> Welcome to the Amogus Shell! Type exit or CTRL+D to exit the shell");
    run();
    printf("\nExiting...\n");
    return 0;
}

