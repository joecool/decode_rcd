#include <math.h>
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
   if(data->cat != 62) {
     printf("Not CAT 062 DATA. Ignore. CAT = %#x\n", data->cat);
     return -1;
   }
   printf("Data CAT 062. ");
   int data_len = ntohs(*(short *)(data->length));
   printf("data length = %d\n", data_len);
   if(data_len != length) {
     printf("Data length is incorrent. Ignore.");
     return -2;
   }

   printf("\nDecoding data.....\n\n");
   char* p = data->data;
  // while(p && p < data->data + data_len) {
      p = decode_062_fspec(p);
  // }

   return 0;
}

char* decode_062_fspec(char* data){
    /* only support FRN is not bigger than 28. */
    unsigned int FRN = 0;
    int count = 0;
    do {
      count++;
      for(int i = 7; i > 0; i--) {
        FRN |= (*data >> i & 1) << (7 * count - i);
      }
      if(count > 5) {
        printf("Fatal Error: FRN is incorrect. Count is %d.\n", count);
        return NULL;
      }
    }while(*(data++) & 1);

    for(int i = 0; i < 28; i++) {
      if((FRN >> i) & 1) {
        decode_items func = funcs[i];
        if(func) data = func(data);
      }
    }

    return data;
}

char* decode_062_010(char* data){
    printf("I062/010 :\n");
    printf("\tSAC = %u, ", *data);
    data++;
    printf("SIC = %u\n", *data);
    data++;
    return data;
}

char* decode_062_015(char* data){
    return data;
}

char* decode_062_070(char* data){
    unsigned int time = (data[0] << 16 & 0xff0000) | (data[1] << 8 & 0xff00) | (data[2] & 0xff);
    printf("I062/070 :\n");
    printf("\tTime of track information: %u, about %f seconds, %f hours.\n", time, time/128.0, time/128.0/3600.0);
    data += 3;
    return data;
}

char* decode_062_105(char* data){
    int latitude = ntohl(*(int*)data);
    data+=4;
    int longtitude = ntohl(*(int*)data);
    data+=4;
    printf("I062/105 :\n");
    printf("\tLatitude : (orignal) %d  ===> %f degrees\n\tLongtitude : (orignal) %d ===> %f degrees\n",
              latitude, latitude * 180.0 / pow(2, 25),
              longtitude, longtitude * 180.0 / pow(2, 25));
    return data;
}

char* decode_062_100(char* data){
  int X = (data[0] << 16 & 0xff0000) | (data[1] << 8 & 0xff00) | (data[2] & 0xff);
  data+=3;
  int Y = (data[0] << 16 & 0xff0000) | (data[1] << 8 & 0xff00) | (data[2] & 0xff);
  data+=3;
  printf("I062/110 :\n");
  printf("\tX : (orignal) %d ===> %f m\n\tY : (orignal) %d ===> %f m\n",
            X, X * 0.5, Y, Y * 0.5);
    return data;
}

char* decode_062_185(char* data){
    short vx = ntohs(*(short *)(data));
    data+=2;
    short vy = ntohs(*(short *)(data));
    data+=2;
    printf("I062/185 :\n");
    printf("\tvx = %f m/s, vy = %f m/s, V(abs) = %f km/h\n", vx * 0.25, vy * 0.25, sqrt((vx * vx + vy * vy) * 0.0625) * 3600 / 1000.0);
    return data;
}

char* decode_062_210(char* data){
    return data;
}

char* decode_062_060(char* data){
    // TODO::
    printf("I062/060 :\n");
    printf("\tHex data is : 0x%02X%02X\n", (unsigned char)data[0], (unsigned char)data[1]);
    data += 2;
    return data;
}

char* decode_062_245(char* data){
    // TODO::
    printf("I062/245 :\n");
    printf("\tHex data is : 0x%02X%02X%02X%02X%02X%02X%02X\n",
            (unsigned char)data[0], (unsigned char)data[1],
            (unsigned char)data[2], (unsigned char)data[3],
            (unsigned char)data[4], (unsigned char)data[5],
            (unsigned char)data[6]);
    data += 7;
    return data;
}

char* decode_062_380(char* data){
  // TODO::
  printf("FIX ME:: I062/380\n");
  return NULL;
}

char* decode_062_040(char* data){
    // TODO::
    printf("I062/040 :\n");
    printf("\tHex data is : 0x%02X%02X\n", (unsigned char)data[0], (unsigned char)data[1]);
    data += 2;
    return data;
}

char* decode_062_080(char* data){
    // TODO::
    printf("I062/80 :\n");
    int count = 1;
    do {
      printf("\tOctet %d: 0x%02X\n", count, (unsigned char)*data);
      if(count > 4) {
        printf("Decode I062/80 failed. Count exceed 4.");
        return NULL;
      }
      count++;
    }while(*data++ & 1);
    return data;
}

char* decode_062_290(char* data){
    return data;
}

char* decode_062_200(char* data){
    return data;
}

char* decode_062_295(char* data){
    return data;
}

char* decode_062_136(char* data){
    short level = ntohs(*(short*)data);
    printf("I062/136 :\n");
    printf("\tMeasured flight level: (orignal)%d ===> %f FL\n", level, level * 0.25);
    data += 2;
    return data;
}

char* decode_062_130(char* data){
    return data;
}

char* decode_062_135(char* data){
    // TODO:: minus value should be recalculated. Need more information.
    short level = ntohs(*(short*)data);
    unsigned int QNH = level & 0x8000;
    level = (level & 0x3fff) | ((level & 0x4000) << 1);
    printf("I062/136 :\n");
    printf("\tQNH correction applied: %u\n", QNH);
    printf("\tBarometric altitude: (orignal)%d ===> %f FL\n", level, level * 0.25);
    data += 2;
    return data;
}

char* decode_062_220(char* data){
    return data;
}

char* decode_062_390(char* data){

    return data;
}

char* decode_062_270(char* data){
    return data;
}

char* decode_062_300(char* data){
    return data;
}

char* decode_062_110(char* data){
    return data;
}

char* decode_062_120(char* data){
    return data;
}

char* decode_062_510(char* data){
    return data;
}

char* decode_062_500(char* data){
    return data;
}

char* decode_062_340(char* data){
    return data;
}
