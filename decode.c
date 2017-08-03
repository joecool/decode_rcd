#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"

int decode(char*, int);

int main(int argc, char* argv[]) {
  char file_name[100];

  if(argc > 1) {
     strncpy(file_name, argv[1], 99);
     printf("file name is %s\n", file_name);

     FILE f = fopen(file_name, "rb");
     if(!f) {
       printf("open file %s failed.\n", file_name);
       return -1;
     }


     # test push
     fclose(f);
  } else {
    printf("Please input file name\n");
  }

  return 0;
}

int decode(char* data, int length) {

  return 0;
}
