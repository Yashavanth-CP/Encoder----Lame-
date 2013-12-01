#include<stdlib.h>
#include<stdio.h>
#include "list.h"

/*List's limited interface implementation */

void addNode(struct node** head, struct node* n) {

    /*When the node to be added is NULL */
    if(n == NULL) 
        return;
    /* When the list is empty() */ 
    if(*head == NULL) { 
        *head = n;
        return;
    }

    /* When list is not empty() */
    n->next = *head;
    *head = n;
};

/*Get the first node from the beginning of the list 
  and update the list's head */

struct node* getNode(struct node** head) {

    if(*head == NULL)
        return NULL;

    struct node* toRet = *head;
    *head = (*head)->next;
    toRet->next = NULL;
    return toRet;
};

/*Print the list */
void printList(struct node** head) {
    
    if(*head == NULL) {
        printf("List is empty() \n");
        return;
    }

    struct node* it = *head;
    while(it != NULL) {
        printf("==>%s", it->filename);
        it = it->next;
    }
    printf("\n");
    return;
}

/*delete the whole list */
void deleteNodes(struct node** head) {

    if(*head == NULL)
        return;

    while(*head != NULL) {
        struct node* toDel = *head;
        *head = (*head)->next;
        free(toDel);
    }
}



