#ifndef _ENCODE_H_
#define _ENCODE_H_
/*Import lame library Interface */
#include<lame/lame.h>

void Encode(char* src);
char* getDestName(char* str);
void Process(char* path, char* name);
#endif
        
