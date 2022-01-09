//
// Created by maozunyao on 2022/1/9.
//

#include <stdbool.h>
#include "variableLInkedLIst.h"

struct node *initNode() {
    struct node *link = (struct node *) malloc(sizeof(struct node));
    link->prev = NULL;
    link->next = NULL;
    link->key = NULL;
    link->arrayOffset = -1;
    link->aRegister = -1;
    return link;
}

struct linklist *initList() {
    struct node *head = (struct node *) malloc(sizeof(struct node));
    struct node *tail = NULL;
    struct linklist *linklist = (struct linklist *) malloc(sizeof(struct linklist));
    linklist->head = head;
    linklist->last = tail;
    return linklist;
}


//insert link at the last location
int insertLast(struct linklist *linklist, char *key) {
    //create a link
    struct node *link = initNode();
    link->key = key;
    int data;
    if (linklist->last == NULL) {
        data = 0;
        linklist->head->next = link;
        link->prev = linklist->head;
        linklist->last = link;
    } else {
        linklist->last->next = link;
        link->prev = linklist->last;
        data = linklist->last->arrayOffset + 4;
    }
    link->arrayOffset = data;
    linklist->last = link;
    return data;
}

void insertParam(struct linklist *linklist, char *key, int count) {
    //create a link
    struct node *link = initNode();
    link->key = key;
    if (linklist->last == NULL) {
        linklist->head->next = link;
        link->prev = linklist->head;
        linklist->last = link;
        link->aRegister = count - 1;
    } else {
        linklist->last->next = link;
        link->prev = linklist->last;
        link->aRegister = count - 1;
    }
    linklist->last = link;
}

struct node *findRegister(struct linklist *linklist, char *key) {
    struct node *current = linklist->head;
    while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->key, key) == 0) {
            return current;
        }
    }
    return NULL;
}

int findOffset(struct linklist *linklist, char *key) {
    //create a link
    struct node *current = linklist->head;
    while (current->next != NULL) {
        current = current->next;
        if (strcmp(current->key, key) == 0) {
            return current->arrayOffset;
        }
    }
    return -1;
}