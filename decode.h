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

static decode_items funcs[28] = {
  decode_062_010,
  0,   /* Should use NULL, but could not be recognized in mingw */
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

char* decode_062_390_TAG(char* data);
char* decode_062_390_CSN(char* data);
char* decode_062_390_IFI(char* data);
char* decode_062_390_FCT(char* data);
char* decode_062_390_TAC(char* data);
char* decode_062_390_WTC(char* data);
char* decode_062_390_DEP(char* data);
char* decode_062_390_DST(char* data);
char* decode_062_390_RDS(char* data);
char* decode_062_390_CFL(char* data);
char* decode_062_390_CTL(char* data);
char* decode_062_390_TOD(char* data);
char* decode_062_390_AST(char* data);
char* decode_062_390_STS(char* data);
char* decode_062_390_STD(char* data);
char* decode_062_390_STA(char* data);
char* decode_062_390_PEM(char* data);
char* decode_062_390_PEC(char* data);

static decode_items funcs_390[18] = {
    decode_062_390_TAG,
    decode_062_390_CSN,
    decode_062_390_IFI,
    decode_062_390_FCT,
    decode_062_390_TAC,
    decode_062_390_WTC,
    decode_062_390_DEP,
    decode_062_390_DST,
    decode_062_390_RDS,
    decode_062_390_CFL,
    decode_062_390_CTL,
    decode_062_390_TOD,
    decode_062_390_AST,
    decode_062_390_STS,
    decode_062_390_STD,
    decode_062_390_STA,
    decode_062_390_PEM,
    decode_062_390_PEC,
};
#endif
