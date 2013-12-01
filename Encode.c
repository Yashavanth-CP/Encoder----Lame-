#include<stdio.h>
#include<stdlib.h>
#include "Encode.h"
#include<string.h>

/* This routine used to modify the name of the file from 
   *.wav to *.mp3 */
char* getDestName(char* src){

            if(src == NULL)
               return NULL;

            char* dest = (char*)malloc(sizeof(char)* strlen(src));

            if(dest == NULL)
               return NULL;
            strcpy(dest, src);           
            char* p = dest;
            int i ; 
            while(*p != '\0'){
//                 printf("%c \t", *p); 
                 p++;
             }
            if(strlen(src) > 3)
              {
                 for(i =0; i<3; i++){
//                 printf("%c \t", *p); 
                     p--;
                  }
              }
            else{
                 return NULL;
              }

            *p = 'm'; p++;
            *p = 'p'; p++;
            *p = '3'; p++;
            *p = '\0';
 
//           printf("After modification is %s \n", dest);
           return dest;
}  

/* Used for trimming the Directory path with / character */

void trim(char* str)
          {
             if(str == NULL)
                return;
 
             char* p = str;
             while(*p != '\0')
                   p++;
 
             p--;
              
             if(*p == '/'){
                char* toDel = (char*) malloc(sizeof(char)* strlen(str));
                strcpy(toDel,str);
                toDel[strlen(toDel)-1] = '\0';
                strcpy(str,toDel);
  //              printf("While trimming is : %s \n",str);
                free(toDel);
            }
}

/* Set the filename with the path also.
   1. inputs(name, path)
   2. Output( path/name)
*/
             
char* SetFullPath(char* name, char* path){

                 if(name == NULL || path == NULL)
                    return NULL;

                 char* fullpath = (char*)malloc(sizeof(char)*(strlen(name)+strlen(path)));
                 
                 if(fullpath == NULL)
                    return NULL;
    //             printf("Before trimming is : %s \n",path);
                 trim(path);
    //             printf("After trimming is : %s \n",path);
                 strcpy(fullpath, path);
                 strcat(fullpath, "/");
                 strcat(fullpath, name);
     //            printf("Full path is %s \n", fullpath);      
                 return fullpath;             
}

/* Main routine to encode the file using lame API ,
   Since the API and library is of no interest to know.
   Did minimal work to encode. However, can be extended
   with additional functionalities */
       
void Encoder(char* src, char* dst){
 
           //printf("Inside Encoder : %s \n",src);
           //printf("Inside Encoder : %s \n",dst);
           if(src == NULL || dst == NULL)
              return;

           int read, write;
           FILE* wav;
           FILE* mp3;
           wav = fopen(src, "rb");
           if(wav == NULL){
              printf(" Unable to open the file %s \n", src);
              return;
            }
           mp3 = fopen(dst, "wb");
           if(mp3 == NULL){
              printf(" Unable to open the file %s \n", dst);
              return;
           } 
           const int PCM_SIZE = 8192;
           const int MP3_SIZE = 8192;
           
           short int PCM_BUFFER[2*PCM_SIZE];
           char MP3_BUFFER[MP3_SIZE];

           lame_t lame = lame_init();
           lame_set_in_samplerate(lame, 44100);
           lame_set_VBR(lame, vbr_default);
           lame_init_params(lame);
           do {
                read = fread(PCM_BUFFER, 2*sizeof(short int), PCM_SIZE, wav);
                if (read == 0)
                write = lame_encode_flush(lame, MP3_BUFFER, MP3_SIZE);
                else
                write = lame_encode_buffer_interleaved(lame, PCM_BUFFER, read, MP3_BUFFER, MP3_SIZE);
                fwrite(MP3_BUFFER, write, 1, mp3);
                } while (read != 0);

           lame_close(lame);
           fclose(wav);
           fclose(mp3);
  
            }

/* Process called from each thread */
void Process(char* path, char* name){

          // char* path = "/home/yash/Desktop/Sounds";
           char* path1 = (char*) malloc(sizeof(char)* strlen(path));
           strcpy(path1,path);
          // char* name;
           char*  srcpath;
           char*  dstfile;
           char*  dstpath;     
     
           if(name != NULL){ 
           srcpath = SetFullPath(name, path1);
           dstfile = getDestName(name);
           }

      //     printf(" SetFullPath :%s \n", dstfile);
       //    printf(" SetFullPath :%s \n", path1);

           if(dstfile != NULL)
              dstpath = SetFullPath(dstfile, path1);

         //  printf(" Srcpath :%s \n", srcpath);
         //  printf(" Dstpath :%s \n", dstpath);
           if(srcpath != NULL && dstpath != NULL)
              Encoder(srcpath, dstpath);
           if(dstfile != NULL)
              free(dstfile);
           if(path1 != NULL)
              free(path1);
}    



