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

void add(List list, char *value);

char *pop(List list);

char *rem(List list);

void print_list(List list);

char *peek(List list);

int is_empty(List list);

int size(List list);

void clear(List list);

int index_of(List list, char *value);

int contains(List list, char *value);

char *get_at(List list, int i);

int replace_at(List list, int i, char *newValue);

int insert_at(List list, int index, char *value);

char *remove_at(List list, int index);

List copy_list(List list);

List sublist(List list, int start, int end);

void save_list(List list, char *fileName);

List load_list(char *fileName);

#endif //SHELL9_LINKEDLIST_H
