#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include "pcre.h"
#include "DirEntries.h"

bool match(char *str){

          if(str == NULL)
             return false;
          int v [15];
          pcre* re;
          const char* error;
          int erroroffset;
          int rc;
          char* regex = "(.*?\\.wav)$";
          re = pcre_compile(regex, 0, &error, &erroroffset, NULL);
         
          if(! re){
                printf("Error in compiling regex \n");
                return false;
          }
          
          rc = pcre_exec(re, NULL, str, strlen(str), 0,0,v, 15);
          pcre_free(re);       
          if(rc < 0){
//             printf("Not matched \n");
             return false;
          }
          else {
  //           printf("Matched %s \n",str);
             return true;
          }
 
    };            

void  ReadDir(char* dirpath, struct node** list){

            DIR* DirHandle;
            struct dirent* Entry;
            
           if((DirHandle = opendir(dirpath)) == NULL){
               printf("Directory cannot be opened \n");
               exit(0);
             }
            
           while((Entry = readdir(DirHandle)) != NULL){
                  //printf("Entry is : %s \n", Entry->d_name);
                  if(Entry->d_type != DT_DIR){
                     bool matched;
                     if((matched = match(Entry->d_name))){
                         struct node* toAdd = (struct node*)malloc(sizeof(struct node));
                         if(toAdd != NULL){
                           strcpy(toAdd->filename,Entry->d_name);
                   //        strcpy(toAdd->path, dirpath);
                           addNode(list, toAdd);
                           }
                        }
                     }
                 } 
         closedir(DirHandle);
} 

#if 0
        DIR* DirHandle;
        struct dirent* Entry;
     
        if((DirHandle = opendir(Argv[1])) == NULL)
           {
             printf(" Directory cannot be opened \n");
             return 1;
           }
 
         while((Entry = readdir(DirHandle)) != NULL)
              { 
                if(Entry->d_type != DT_DIR)
                printf(" File name is : %s and length is %d \n", Entry->d_name, Entry->d_type);
                bool matched = match(Entry->d_name);
              }
          closedir(DirHandle);

#endif        

