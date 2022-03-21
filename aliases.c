#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include "linkedList.h"
#include "shell.h"

List aliases;
char *aliasFileName = "aliases.aliases";

void saveAliases() {
    chdir(savingPath);
    chdir("..");
    if (access(aliasFileName, F_OK) == 0) save_list(aliases, aliasFileName);
    else aliases = new_list();
}

void loadAliases() {
    if (access(aliasFileName, F_OK) == 0) aliases = load_list(aliasFileName);
    else {
        printf("No aliases file found, no aliases will be saved when exiting!\n");
        aliases = new_list();
    }
}

void addToAlias(char **commandArray) {
    if (commandArray[1] == NULL) {
        if (is_empty(aliases)) printf("There are no aliases set.\n");
        else print_list(aliases);
        return;
    }
    if (commandArray[2] == NULL) {
        printf("The alias command takes either zero or two arguments, but I received less than that. Try alias <name> <command>\n");
        return;
    }
    char *parsedInput = malloc(sizeof(char) * 512);
    parsedInput[0] = 0;
    int i = 1;
    while (commandArray[i] != NULL) {
        strcat(parsedInput, commandArray[i]);
        if (commandArray[i + 1] != NULL)
            strcat(parsedInput, " ");
        i++;
        printf("%s\n", parsedInput);
    }
    strcat(parsedInput, "\0");
    push(aliases, parsedInput);
}



