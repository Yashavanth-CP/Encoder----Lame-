#define  _GNU_SOURCE //required for setting the CPU affinity.
#include<sched.h> // For setting the affinity of each thread. 
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

/* Global list shared between multiple threads */
struct node* list = NULL;

/*mutex m is shared between threads for accessing the
  shared list ..Details on list elements can be found
  in list.h */

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

/* thread related data structure.. holds its ID and the directory path */
struct ID {
    int data;
    char path[256];
};

/*Thread start routine -- 
  1. get the node from  the shared list
  2. Encode the source format to destination format 
  3. free the node 
  4. if list is not empty -- get a node and process.

*/

void* EntryPoint(void *d) {

    struct ID* thid = (struct ID*) d;
//  printf("Thread id is : %d", thid->data);
    int i, s, cores;

    /* structure to set the affinity for the thread */
    cpu_set_t cpuset;
    pthread_t self = pthread_self();
    s = pthread_getaffinity_np(self, sizeof(cpu_set_t), &cpuset);
    if(s != 0)
        printf("Error pthread_getaffinity_np \n");

    /* Syscall to get the number of cores on the machine */
    cores = sysconf(_SC_NPROCESSORS_ONLN);
    for(i =0; i <cores; i++) {
        if(CPU_ISSET(i, &cpuset))
            printf(" Set to cpu %d \n",i);
    }

    while(1) {
        pthread_mutex_lock(&m);
        struct node* it = list;
        if(list != NULL)
            list = list->next;
        pthread_mutex_unlock(&m);

        if(it == NULL) {
            break;
        }
        /* Call to the encoding process routine */
        Process(thid->path, it->filename);
        free(it); /* Free the memory */
    }
    free(thid); /* Free the structure */
}


/* Called by main thread ---Route to
           1. To Check the number of cores on the machine 
           2. Create as many number of threads as required.
           3. Wait for the threads to join, once all are finished.
*/

void Create_Threads(char* dirpath) {

    int i, cores;
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    
   /* Get the number of cores on the machine */
    cores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Number of cores are: %d", cores);
    for(i =0; i<cores; i++)
        CPU_SET(i, &cpuset);

    int s;
    pthread_t th[cores];
    
    /*Create the threads */
    for(i = 0; i < cores; i++) {
        struct ID* thid = (struct ID*) malloc(sizeof(struct ID));
        thid->data = i;
        strcpy(thid->path, dirpath);
        pthread_create(&th[i], NULL, &EntryPoint,(void*)thid);
        s = pthread_setaffinity_np(th[i], sizeof(cpu_set_t), &cpuset);
        if(s != 0)
            printf("Error: setAffinity \n");
    }

    /*Wait for all thread to join() */
    for(i = 0; i <cores; i++) {
        pthread_join(th[i], NULL);
    }
}


int main(int Argc, char** Argv) {

    if(Argc != 2) {
        printf("Invalid Usage: Number of Arguments are invalid\n");
        printf("Usage is : ./main Path-to-directory \n");
        exit(0);
    }

    printf(" Number of Arguements are: %d \n", Argc);
    printf(" Directory path is :  %s \n", Argv[1]);
    
    /*Call routine to read the dircectory and build the list */
    ReadDir(Argv[1],&list);
    Create_Threads(Argv[1]);
    //printList(&list);
    deleteNodes(&list);/*Redundant -- but still for safety */ 
    // printList(&list);

    return 0;
}


