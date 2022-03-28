/* list.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

List new_list() {
    List list = malloc(sizeof(List));
    *list = NULL;
    return list;
}

Node *new_node(char *value) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

void delete_node(Node *node) {
    free(node);
}

void push(List list, char *value) {
    Node *newTail = new_node(value);
    if (*list == NULL) {
        *list = newTail;
    } else {
        Node *tail = *list;
        while (tail->next != NULL)
            tail = tail->next;
        tail->next = newTail;
    }
}

void print_list(List list) {
    printf("[");
    Node *curr = *list;
    while (curr != NULL) {
        printf("\"%s\"", curr->value);
        if (curr->next != NULL)
            printf(", ");
        curr = curr->next;
    }
    printf("]\n");
}

int is_empty(List list) {
    return size(list) == 0;
}

int size(List list) {
    if (*list == NULL)
        return 0;
    int counter = 1;
    Node *curr = *list;
    while (curr->next != NULL) {
        counter++;
        curr = curr->next;
    }
    return counter;
}

int index_of(List list, char *value) {
    if (is_empty(list))
        return -1;
    int counter = 0;
    Node *curr = *list;
    while (curr != NULL) {
        if (strcmp(curr->value, value) == 0)
            return counter;
        curr = curr->next;
        counter++;
    }
    return -1;
}

int contains(List list, char *value) {
    return index_of(list, value) != -1;
}

char *get_at(List list, int i) {
    if (is_empty(list))
        return NULL;
    int counter = 0;
    Node *curr = *list;
    while (counter++ != i) {
        if (curr == NULL)
            return NULL;
        curr = curr->next;
    }
    if (curr == NULL)
        return NULL;
    return curr->value;
}

char *remove_at(List list, int index) {
    if (size(list) < index || is_empty(list) || index < 0)
        return NULL;
    if (index == 0) {
        Node *oldFirst = *list;
        *list = oldFirst->next;
        char *val = oldFirst->value;
        delete_node(oldFirst);
        return val;
    }
    Node *prev = *list;
    int counter = 1;
    while (prev != NULL && counter < index) {
        prev = prev->next;
        counter++;
    }
    if (prev == NULL)
        return NULL;
    if (prev == NULL || prev->next == NULL)
        return NULL;
    Node *toBeDeleted = prev->next;
    char *removed = toBeDeleted->value;
    prev->next = toBeDeleted->next;
    delete_node(toBeDeleted);
    return removed;
}

void save_list(List list, char *fileName) {
    FILE *file;
    file = fopen(fileName, "w+");
    if (file) {
        for (int i = 0; i < size(list); i++)
            fprintf(file, "%s\n", get_at(list, i));
        fclose(file);
    }
}

List load_list(char *fileName) {
    List list = new_list();
    FILE *file;
    file = fopen(fileName, "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            char *curLine = malloc((strlen(line) - 1) * sizeof(char));
            strncpy(curLine, line, strlen(line) - 1);
            curLine[strlen(line) - 1] = '\0';
            push(list, curLine);
        }
        fclose(file);
    }
    return list;
}