#include<stdlib.h>
#include<stdio.h>
#include "list.h"
void addNode(struct node** head, struct node* n){

            if(n == NULL)
               return;

            if(*head == NULL){
                *head = n;
                 return;
               }
  
              n->next = *head;
              *head = n;
};

struct node* getNode(struct node** head){

            if(*head == NULL)
               return NULL;

            struct node* toRet = *head;
            *head = (*head)->next;
            toRet->next = NULL;
            return toRet;
};

void printList(struct node** head){
                printf("printNodes called \n");
               if(*head == NULL){
                  printf("List is empty() \n");
                  return;
                 }
 
               struct node* it = *head;
               while(it != NULL){
                     printf("==>%s", it->filename);
                     it = it->next;
                    }
               printf("\n");
               return;
}
         
void deleteNodes(struct node** head){

                if(*head == NULL)
                    return;
                
                 while(*head != NULL){
                 struct node* toDel = *head;
                        *head = (*head)->next;
                         free(toDel);
                 }                                          
}

 

