//
// Created by drb20147 on 21/03/2022.
//

#ifndef SHELL9_ALIASES_H
#define SHELL9_ALIASES_H

void addToAlias(char **commandArray);

void loadAliases();

void saveAliases();

int tryToRunAliasCommand(char *command);

#endif
