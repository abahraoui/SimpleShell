#ifndef _HISTORY_FUNCTIONS
#define _HISTORY_FUNCTIONS

char *history[20]; /// created a history string array.
int historyCounter; /// created a counter for vacant entries to store commands.

void addToHistory(char *commandArray[50]);

void printHistory();

void executePreviousCommand();

void executeNthCommand(char *commandArray[50]);

void executeInverseNthCommand(char *commandArray[50]);

void executeHistoryInvocation(char *commandArray[50]);

int getNumberFromString(char *input);

#endif