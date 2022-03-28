#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "linkedList.h"
#include "shell.h"
#include <ctype.h>

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
        for (int i = 0; i < size(aliases); ++i) {
            char *trimmedAlias = trimString(strdup(get_at(aliases, i)));
            char *e = strchr(trimmedAlias, ' ');
            if (e == NULL)
                remove_at(aliases, i--);
        }
        printf("Loaded %d aliases correctly from file:\n", size(aliases));
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
    if (strcmp(commandArray[1], "unalias") == 0 || strcmp(commandArray[1], "alias") == 0) {
        printf("%s\n", "You can't use this alias, reserved elsewhere.");
        return;
    }
    if (contains(aliasKeys(), commandArray[1])) {
        printf("%s\n", "Warning, this alias is already mapped. Overwriting...");
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
    for(int j = 0; j < size(aliases); j++){
        if(strcmp(get_at(aliasKeys(), j),nameParsed) == 0){
            remove_at(aliases,j);
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

        int j = 1;
        while (command[j] != NULL){
            commandArray[i] = command[j];
            j++;
            i++;
        }
        commandArray[i] = NULL;

        int print =0;
        char tempStr[100] = "";
        while (commandArray[print] != NULL){
            strcat(tempStr, commandArray[print]);
            strcat(tempStr, " ");
            print++;
        }
        printf("The Alias you are executing is: %s \n", tempStr);

        readInput(commandArray);
        return 1;
    }
    return -1;
}

void removeAlias(char **commandArray) {
    if (commandArray[1] == NULL) {
        printf("The unalias command takes one parameter, but I received zero. Try unalias <name>\n");
        return;
    } else if (commandArray[2] != NULL) {
        printf("The unalias command takes one parameter, but I received more than that. Try unalias <name>\n");
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
    bool check = false;
    strcat(parsedInput, "\0");
    char *nameParsed =  malloc(sizeof(char) * 512);
    strcpy( nameParsed,parsedInput);
    nameParsed = strtok(nameParsed, " ");
    for(int j = 0; j < size(aliases); j++){
        if(strcmp(get_at(aliasKeys(), j),nameParsed) == 0){
            remove_at(aliases,j);
            check = true;
        }
    }
    if (check == true) {
        printf("Alias \"%s\" has been removed.\n", commandArray[1]);
    } else {
        printf("Alias \"%s\" does not exist.\n", commandArray[1]);
    }
}

char *trimString(char *str) {
    size_t len = strlen(str);
    while (isspace(str[len - 1]))
        --len;
    while (*str && isspace(*str))
        ++str, --len;
    return strndup(str, len);
}