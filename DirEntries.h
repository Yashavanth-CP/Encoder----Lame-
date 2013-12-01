#ifndef _DIR_ENTRIES_H_
#define _DIR_ENTRIES_H_
#include "list.h"
#include<stdbool.h>
bool match(char* str);
void ReadDir(char* dirpath, struct node** head);

#endif



