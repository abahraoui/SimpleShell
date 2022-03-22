#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "linkedList.h"
#include "shell.h"

List aliases;
char *aliasFileName = ".aliases";

void saveAliases() {
    chdir(savingPath);
    if (access(aliasFileName, F_OK) != 0){
        printf("Alias file was not found or doesn't exist, creating a new one at %s/..\n", savingPath);
    }
    save_list(aliases, aliasFileName);
}

void loadAliases() {
    if (access(aliasFileName, F_OK) == 0){

        aliases = load_list(aliasFileName);
        printf("loaded aliases correctly:\n");
        print_list(aliases);
    }
    else {
        printf("Aliases file not found or doesn't exist.\n");
        aliases = new_list();
    }
}
List aliasKeys() {
    List keys = new_list();
    for (int i = 0; i < size(aliases); ++i) {
        char *alias = malloc(sizeof(char) * 512);
        strcpy(alias, get_at(aliases, i));
        push(keys, strtok(alias, " "));
    }
    return keys;
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
    if(strcmp(commandArray[1], "unalias") == 0 || strcmp(commandArray[1], "alias") == 0 ){
        printf("%s\n","You can't use this alias, reserved elsewhere.");
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
    }
    strcat(parsedInput, "\0");
    char *nameParsed =  malloc(sizeof(char) * 512);
            strcpy( nameParsed,parsedInput);
    nameParsed = strtok(nameParsed, " ");
    for(int i = 0; i < size(aliases); i++){
        if(strcmp(get_at(aliasKeys(), i),nameParsed) == 0){
            remove_at(aliases,i);
        }
    }
    push(aliases, parsedInput);
}


char *aliasCommand(int index) {
    char *command = malloc(sizeof(char) * 512);
    strcpy(command, strstr(get_at(aliases, index), " "));
    return command;
}

int tryToRunAliasCommand(char *command[]) {
    int indexOfCommand = index_of(aliasKeys(), command[0]);
    if (indexOfCommand != -1) {
        char *commands[20];
        commands[0] = aliasCommand(indexOfCommand) + 1;
        commands[1] = NULL;
        char *commandArray[100];
        const char *delimiter = " ;\t|><&\n";
        const char *token;
        /* get the first token */
        token = strtok(commands[0], delimiter);
        /* walk through other tokens */
        int i = 0;
        while (token != NULL) {
            commandArray[i] = token;
            token = strtok(NULL, delimiter);
            i++;
        }
//        commandArray[i] = NULL;


////this code right here is my attempt of making it take params, however i have to go to do work and it is not working.
////if you try to solve this dont worry about it, delete only if you make it work with params

        printf("%s",command[1]);
        int j = 1;
        while (command[j] != NULL){
            commandArray[i] = command[j];
            j++;
            i++;
        }
        commandArray[i] = NULL;


        readInput(commandArray);
        return 1;
    }
    return -1;
}

void removeAlias(char **commandArray) {
    char *parsedInput = malloc(sizeof(char) * 512);
    parsedInput[0] = 0;
    int i = 1;
    while (commandArray[i] != NULL) {
        strcat(parsedInput, commandArray[i]);
        if (commandArray[i + 1] != NULL)
            strcat(parsedInput, " ");
        i++;
    }
    bool check = false;
    strcat(parsedInput, "\0");
    char *nameParsed =  malloc(sizeof(char) * 512);
    strcpy( nameParsed,parsedInput);
    nameParsed = strtok(nameParsed, " ");
    for(int i = 0; i < size(aliases); i++){
        if(strcmp(get_at(aliasKeys(), i),nameParsed) == 0){
            remove_at(aliases,i);
            check = true;
        }
    }
    if(check == true){
        printf("%s\n","Aliases has been removed.");
    }else {
        printf("%s\n","Alias does not exist");
    }

}


