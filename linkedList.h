//
// Created by drb20147 on 21/03/2022.
//

#ifndef SHELL9_LINKEDLIST_H
#define SHELL9_LINKEDLIST_H

/* list.h */
typedef struct Node {
    char *value; // "p pwd"
    struct Node *next;
} Node;

typedef Node **List;

List new_list();

void delete_node(Node *node);

Node *new_node(char *value);

void push(List list, char *value);

void print_list(List list);

int is_empty(List list);

int size(List list);

int index_of(List list, char *value);

int contains(List list, char *value);

char *get_at(List list, int i);

char *remove_at(List list, int index);

void save_list(List list, char *fileName);

List load_list(char *fileName);

#endif //SHELL9_LINKEDLIST_H
