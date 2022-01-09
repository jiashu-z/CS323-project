//
// Created by maozunyao on 2022/1/9.
//

#ifndef CS323_PROJECT1_VARIABLELINKEDLIST_H
#define CS323_PROJECT1_VARIABLELINKEDLIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node {
    char *key;
    int arrayOffset;
    int aRegister;
    struct node *next;
    struct node *prev;
};

struct linklist {
    struct node *head;  //stores the head, the first element is head->next
    struct node *last; //stores the last element
};

//is list empty
struct linklist *initList();


//insert link at the last location
int insertLast(struct linklist *linklist, char *key);

int findOffset(struct linklist *linklist, char *key);

void insertParam(struct linklist *linklist, char *key, int count);

struct node *findRegister(struct linklist *linklist, char *key);


#endif //CS323_PROJECT1_VARIABLELINKEDLIST_H
