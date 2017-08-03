#ifndef __DECODE_H__
#define __DECODE_H__

typedef struct __attribute__((packed)) _data {
    int seconds;
    short mseconds;
    short length;
    char logicno;
    char channel;
    char data[0];
}DATA, *PDATA;

typedef struct __attribute__((packed)) _sdps {



}SDPS, *PSDPS;

enum {
   I062_010 = 1,
   SPARE1,
   I062_015,
   I062_070,
   I062_105,
   I062_100,
   I062_185,
}FRN;



int decode(PDATA data, int length);
int decode_062(PSDPS data, int length);

#endif
