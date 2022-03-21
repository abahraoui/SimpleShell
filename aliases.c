#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
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


List aliasValues() {
    List values = new_list();
    for (int i = 0; i < size(aliases); ++i) {
        char *alias = malloc(sizeof(char) * 512);
        strcpy(alias, get_at(aliases, i));

        const char *delimiter = " ;\t|><&\n";
        const char *token;
        char *parsedInput = malloc(sizeof(char) * 512);
        parsedInput[0] = 0;
        /* get the first token */
        token = strtok(alias, " ");
        /* walk through other tokens */
        int i = 0;
        while (token != NULL) {
            if (i == 0) {
                token = strtok(NULL, delimiter);
                i++;
                continue;
            }

            parsedInput = strcat(parsedInput, token);
            token = strtok(NULL, delimiter);
            i++;
        }
        strcat(parsedInput, "\0");


        push(values, strtok(parsedInput, " "));

    }
    return values;
}

char *aliasCommand(int index) {
    char *command = malloc(sizeof(char) * 512);
    strcpy(command, strstr(get_at(aliases, index), " "));
    return command;
}

int tryToRunAliasCommand(char *command) {
    int indexOfCommand = index_of(aliasKeys(), command);
    if (indexOfCommand != -1) {
        char *commands[20];
        commands[0] = aliasCommand(indexOfCommand) + 1;
        commands[1] = NULL;
        char *commandArray[50];
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
        commandArray[i] = NULL;
        readInput(commandArray);
        return 1;
    }
    return -1;
}

void removeAlias(char **commandArray) {
    int i = 1;
    char *parsedInput = malloc(sizeof(char) * 512);
    parsedInput[0] = 0;
    while (commandArray[i] != NULL) {
        strcat(parsedInput, commandArray[i]);

        i++;
    }
    strcat(parsedInput, "\0");
    bool check = false;
    for(int i = 0; i < size(aliases); i++){
        int indexOfCommand = index_of(aliasValues(), parsedInput);
        if (indexOfCommand != -1) {
            if(remove_at(aliases, indexOfCommand)!=NULL){
                  check = true;
            }
        }
    }
    if(check == true){
        printf("%s\n","Aliases has been removed.");
    }else {
        printf("%s\n","Alias does not exist");
    }

}


