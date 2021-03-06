#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include "pcre.h"
#include "DirEntries.h"

/* Check whether the file ends with .wav extension
   using PCRE
*/
bool match(char *str) {

    if(str == NULL)
        return false;

    int v [15];
    pcre* re;
    const char* error;
    int erroroffset;
    int rc;

    char* regex = "(.*?\\.wav)$";
    re = pcre_compile(regex, 0, &error, &erroroffset, NULL);

    if(! re) {
        printf("Error in compiling regex \n");
        return false;
    }

    rc = pcre_exec(re, NULL, str, strlen(str), 0,0,v, 15);
    pcre_free(re);

    if(rc < 0) {
        //         printf("Not matched \n");
        return false;
    }
    else {
        //         printf("Matched %s \n",str);
        return true;
    }

};


/* Read the user provided Directory and list its contents
   only files are used for matching purposes
*/

void  ReadDir(char* dirpath, struct node** list) {

    DIR* DirHandle;
    struct dirent* Entry;

    if((DirHandle = opendir(dirpath)) == NULL) {
        printf("Directory cannot be opened \n");
        exit(0);
    }

    while((Entry = readdir(DirHandle)) != NULL) {
        //printf("Entry is : %s \n", Entry->d_name);
        if(Entry->d_type != DT_DIR) {
            bool matched;
            if((matched = match(Entry->d_name))) {
                struct node* toAdd = (struct node*)malloc(sizeof(struct node));
                if(toAdd != NULL) {
                    strcpy(toAdd->filename,Entry->d_name);
                    //        strcpy(toAdd->path, dirpath);
                    addNode(list, toAdd);
                }
            }
        }
    }
    closedir(DirHandle);
}


