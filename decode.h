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
    char cat;
    char length[2];
    char data[0];
}SDPS, *PSDPS;

int decode(PDATA data, int length);
int decode_062(PSDPS data, int length);

char* decode_062_fspec(char* data);
char* decode_062_010(char* data);
char* decode_062_015(char* data);
char* decode_062_070(char* data);
char* decode_062_105(char* data);
char* decode_062_100(char* data);
char* decode_062_185(char* data);
char* decode_062_210(char* data);
char* decode_062_060(char* data);
char* decode_062_245(char* data);
char* decode_062_380(char* data);
char* decode_062_040(char* data);
char* decode_062_080(char* data);
char* decode_062_290(char* data);
char* decode_062_200(char* data);
char* decode_062_295(char* data);
char* decode_062_136(char* data);
char* decode_062_130(char* data);
char* decode_062_135(char* data);
char* decode_062_220(char* data);
char* decode_062_390(char* data);
char* decode_062_270(char* data);
char* decode_062_300(char* data);
char* decode_062_110(char* data);
char* decode_062_120(char* data);
char* decode_062_510(char* data);
char* decode_062_500(char* data);
char* decode_062_340(char* data);

typedef char* (*decode_items)(char *);

decode_items funcs[28] = {
  decode_062_010,
  NULL,
  decode_062_015,
  decode_062_070,
  decode_062_105,
  decode_062_100,
  decode_062_185,
  decode_062_210,
  decode_062_060,
  decode_062_245,
  decode_062_380,
  decode_062_040,
  decode_062_080,
  decode_062_290,
  decode_062_200,
  decode_062_295,
  decode_062_136,
  decode_062_130,
  decode_062_135,
  decode_062_220,
  decode_062_390,
  decode_062_270,
  decode_062_300,
  decode_062_110,
  decode_062_120,
  decode_062_510,
  decode_062_500,
  decode_062_340,
};

#endif
