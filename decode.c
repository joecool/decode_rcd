#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"

int main(int argc, char* argv[]) {
  char file_name[100];

  if(argc > 1) {
     strncpy(file_name, argv[1], 99);
     printf("file name is %s\n", file_name);

     FILE* f = fopen(file_name, "rb");
     if(!f) {
       printf("open file %s failed.\n", file_name);
       return -1;
     }

     while(!feof(f)) {
       PDATA header = malloc(sizeof(DATA));
       int len = fread(header, 1, sizeof(DATA), f);
       if(len != sizeof(DATA)) {
         printf("Get header failed. len = %d.\n", len);
         free(header);
         break;
       }
       header = realloc(header, sizeof(DATA) + header->length + 4);
       len = fread(header->data, 1, header->length + 4, f);
       if (len != header->length + 4) {
         printf("Get data content failed.\n");
         free(header);
         break;
       }

       decode(header, sizeof(DATA) + header->length + 4);

       free(header);
       getchar();
     }
     fclose(f);
  } else {
    printf("Please input file name\n");
  }

  return 0;
}

int decode(PDATA data, int length) {
  printf("seconds = %d, ", data->seconds);
  printf("msc = %d, ", data->mseconds);
  printf("length = %d, ", data->length);
  printf("logic = %d, ", data->logicno);
  printf("channel = %d\n", data->channel);

  if(data->logicno == 1) {
    char* buffer = malloc(data->length + 1);
    memcpy(buffer, data->data, data->length);
    buffer[data->length] = 0;
    printf("%s", buffer);
  } else if(data->logicno == 2) {
    decode_062((PSDPS)(data->data), data->length);
  }
  return 0;
}

int decode_062(PSDPS data, int length) {


}
