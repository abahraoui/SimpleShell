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

char *pop(List list) {
    if (*list == NULL) {
        delete_node(*list);
        return NULL;
    } else if ((*list)->next == NULL) {
        char *tailValue = (*list)->value;
        free(*list);
        *list = NULL;
        return tailValue;
    }
    Node *beforeTail = *list;
    while (beforeTail->next->next != NULL)
        beforeTail = beforeTail->next;
    char *oldTailValue = beforeTail->next->value;
    free(beforeTail->next);
    beforeTail->next = NULL;
    return oldTailValue;
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

char *peek(List list) {
    if (*list == NULL)
        return NULL;
    else if ((*list)->next == NULL)
        return (*list)->value;
    Node *tail = *list;
    while (tail->next != NULL)
        tail = tail->next;
    return tail->value;
}

char *rem(List list) {
    if (*list == NULL)
        return NULL;
    char *val = (*list)->value;
    if ((*list)->next == NULL) {
        free(*list);
        *list = NULL;
    } else
        *list = (*list)->next;
    return val;
}

void add(List list, char *value) {
    Node *node = new_node(value);
    node->next = *list;
    *list = node;
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

void clear(List list) {
    while (!is_empty(list))
        rem(list);
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

int replace_at(List list, int i, char *newValue) {
    if (is_empty(list))
        return 0;
    int counter = 0;
    Node *curr = *list;
    while (counter++ != i) {
        if (curr == NULL)
            return 0;
        curr = curr->next;
    }
    if (curr == NULL)
        return 0;
    curr->value = newValue;
    return 1;
}

int insert_at(List list, int index, char *value) {
    if (size(list) < index || index < 0)
        return 0;
    Node *newNode = new_node(value);
    if (index == 0) {
        if (!is_empty(list)) {
            Node *first = *list;
            newNode->next = first;
        }
        *list = newNode;
        return 1;
    }
    Node *prev = *list;
    int counter = 1;
    while (prev != NULL && counter < index) {
        prev = prev->next;
        counter++;
    }
    newNode->next = prev->next;
    prev->next = newNode;
    return 1;
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

List copy_list(List list) {
    return sublist(list, 0, size(list));
}

List sublist(List list, int start, int end) {
    if (!(start >= 0 && end <= size(list) && start < end))
        return NULL;
    List copy = new_list();
    while (start != end)
        push(copy, get_at(list, start++));
    return copy;
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