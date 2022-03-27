//
// Created by ykb20128 on 27/01/2022.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"

const char *originalPath;

void saveOriginalPath() {
    originalPath = getenv("PATH");
    printf("originalPath: %s\n", originalPath);
}

void restoreOriginalPath() {
    printf("current: %s\n", getenv("PATH"));
    setenv("PATH", originalPath, 1);
    printf("restored: %s\n", getenv("PATH"));
    chdir(originalPath);
}

int main(void) {
    saveOriginalPath();
    printf("%s\n", "> Welcome to the CS210 Shell! Type exit or CTRL+D to exit the shell");
    run();
    printf("\nExiting...\n");
    restoreOriginalPath();
    return 0;
}

