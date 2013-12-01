#define  _GNU_SOURCE
#include<sched.h>
#include "list.h"
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<ctype.h>
#include<unistd.h>
#include<errno.h>
#include "DirEntries.h"
#include "Encode.h"
struct node* list = NULL;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;


struct ID{
           int data;
           char path[256];
 };


void* thread_display(void *d){

          struct ID* thid = (struct ID*) d;
//          printf("Thread id is : %d", thid->data);
          int i, s, cores;

          cpu_set_t cpuset;
          pthread_t self = pthread_self();
          s = pthread_getaffinity_np(self, sizeof(cpu_set_t), &cpuset);
          if(s != 0)
             printf("Error pthread_getaffinity_np \n");
          cores = sysconf(_SC_NPROCESSORS_ONLN);
//          cores = 1;
          for(i =0; i <cores; i++){
              if(CPU_ISSET(i, &cpuset))
                 printf(" Set to cpu %d \n",i);
          }
          
          while(1){
          pthread_mutex_lock(&m);
          struct node* it = list;
          if(list != NULL)
             list = list->next;
          pthread_mutex_unlock(&m); 
//          while(it != NULL && it->explored == 1)
  //              it = it->next;
           
          if(it == NULL){
             break;
           }
         
          Process(thid->path, it->filename); 
  //        printf(" The value in this thread %s is %d \n",thid->path, thid->data);
          //it->explored =1;
          free(it);
   //       sleep(1);
         }
          free(thid);
}
                   

void Create_Threads(char* dirpath){
           
           int i, cores;
           cpu_set_t cpuset;
           CPU_ZERO(&cpuset);

           cores = sysconf(_SC_NPROCESSORS_ONLN);
  //         cores =1;
           printf("Number of cores are: %d", cores);
           for(i =0; i<cores; i++)
              CPU_SET(i, &cpuset);

           int s;
           pthread_t th[cores];
           for(i = 0; i < cores; i++){
           struct ID* thid = (struct ID*) malloc(sizeof(struct ID));
           thid->data = i;
           strcpy(thid->path, dirpath);
           pthread_create(&th[i], NULL, &thread_display,(void*)thid);
           s = pthread_setaffinity_np(th[i], sizeof(cpu_set_t), &cpuset);
           if(s != 0)
             printf("Error: setAffinity \n");

           }
          
           for(i = 0; i <cores; i++){
           pthread_join(th[i], NULL);
           }
      }

int main(int Argc, char** Argv){

           if(Argc != 2){
              printf("Invalid Usage: Number of Arguements are invalid\n");
              printf("Usage is : ./main Path-to-directory \n");
              exit(0);
            }

           printf(" Number of Arguements are: %d \n", Argc);
           printf(" Directory path is :  %s \n", Argv[1]);
           
        
           struct node* toAdd;
           int i,cores;
           cores = sysconf(_SC_NPROCESSORS_ONLN);
           
           printf("Number of cores are: %d", cores);
           ReadDir(Argv[1],&list);
          #if 0 
           for(i=0; i < 25; i++){
           toAdd = (struct node*) malloc(sizeof(struct node));
           toAdd->data = i;
           toAdd->explored = 0;
           toAdd->next  = NULL;
           addNode(&list, toAdd);  
           }
          #endif  
           Create_Threads(Argv[1]);
           //printList(&list);
           deleteNodes(&list); 
          // printList(&list);  

          return 0;
} 

         
