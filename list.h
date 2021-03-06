#ifndef _LIST_H_
#define _LIST_H_
#include<stdlib.h>
#include<stdio.h>
/* List's each individual structure */
struct node {
    char filename[64];
    struct node* next;
};

/* Lists interface */
void addNode(struct node** head, struct node*n);
struct node* getNode(struct node** head);
void printList(struct node** head);
void deleteNodes(struct node** head);
#endif


