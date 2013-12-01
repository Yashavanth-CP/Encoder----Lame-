#ifndef _DIR_ENTRIES_H_
#define _DIR_ENTRIES_H_
#include "list.h"
#include<stdbool.h>
#if 0
typedef int bool;
const bool true =1;
const bool false = 0;
#endif
bool match(char* str);
void ReadDir(char* dirpath, struct node** head);

#endif



