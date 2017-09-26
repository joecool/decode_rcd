#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode.h"

int decode(PDATA data, int length) {
  printf("\n========================= Message received =========================\n\n");
  printf("seconds = %d, ", data->seconds);
  printf("msc = %d, ", data->mseconds);
  printf("length = %d, ", data->length);
  printf("logic = %d, ", data->logicno);
  printf("channel = %d\n", data->channel);

  if(data->logicno == 1) {
    char* buffer = malloc(data->length + 1);
    memcpy(buffer, data->data, data->length);
    buffer[data->length] = 0;
    printf("%s\n\n", buffer);
  } else if(data->logicno == 2) {
    decode_062((PSDPS)(data->data), data->length);
  }
  printf("\n========================= Message ended =========================\n\n");
  return 0;
}

int decode_062(PSDPS data, int length) {
   if(data->cat != 62) {
     printf("Not CAT 062 DATA. Ignore. CAT = %#x\n", data->cat);
     return -1;
   }
   printf("\nData CAT 062. \n");
   int data_len = ntohs(*(short *)(data->length));
   printf("data length = %d\n", data_len);
   if(data_len != length) {
     printf("Data length is incorrent. Ignore.");
     return -2;
   }
   char* p = data->data;
   int num = 1;
   while(p && p < data->data + data_len - 3) {
      printf("\n############### Decoding FSPEC %d ################\n", num++);
      p = decode_062_fspec(p);
   }

   if(!p) {
       printf("Fatal Error. Decode failed.\n");
       return -1;
   }

   if(p[0] == 'E' && p[1] == 'N' && p[2] == 'D' && p[3] == '!') {
       printf("\nMessage ended correctly.\n\n");
   } else {
       printf("\nWarning: Message has NO \"END!\" symbol.\n\n");
   }
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

    printf("FSEPC FRN list: %#X\n", FRN);
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
    short ax = ntohs(*(short *)(data));
    data+=2;
    short ay = ntohs(*(short *)(data));
    data+=2;
    printf("I062/210 :\n");
    printf("\tAx = %f m/s^2, Ay = %f m/s^2\n", ax * 0.25, ay * 0.25);
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
    printf("I062/080 :\n");
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
    // TODO::
    printf("FIX ME:: I062/290\n");
    return NULL;
}

char* decode_062_200(char* data){
    // TODO::
    printf("I062/200 :\n");
    printf("\tHex data is : 0x%02X\n", (unsigned char)data[0]);
    data++;
    return data;
}

char* decode_062_295(char* data){
    // TODO::
    printf("FIX ME:: I062/295\n");
    return NULL;
}

char* decode_062_136(char* data){
    short level = ntohs(*(short*)data);
    printf("I062/136 :\n");
    printf("\tMeasured flight level: (orignal)%d ===> %f FL\n", level, level * 0.25);
    data += 2;
    return data;
}

char* decode_062_130(char* data){
    short altitude = ntohs(*(short*)data);
    printf("I062/130 :\n");
    printf("\tCalculated track geometric altitude: (orignal)%d ===> %f ft\n", altitude, altitude * 6.25);
    data += 2;
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
    short rate = ntohs(*(short*)data);
    printf("I062/220 :\n");
    printf("\tCalculated rate of climb/descent: (orignal)%d ===> %f feet/min\n", rate, rate * 6.25);
    data += 2;
    return data;
}

char* decode_062_390(char* data){
    unsigned int primary_field = 0;
    int count = 0;
    printf("I062/390 :\n");
    do {
      count++;
      for(int i = 7; i > 0; i--) {
        primary_field |= (*data >> i & 1) << (7 * count - i);
      }
      if(count > 3) {
        printf("Fatal Error: primary_field is incorrect. Count is %d.\n", count);
        return NULL;
      }
    }while(*(data++) & 1);

    for(int i = 0; i < 28; i++) {
      if((primary_field >> i) & 1) {
        decode_items func = funcs_390[i];
        if(func) data = func(data);
      }
    }
    return data;
}

char* decode_062_390_TAG(char* data){
    printf("\tFPPS Identification Tag:\n");
    printf("\t\tSAC = %u, ", *data);
    data++;
    printf("SIC = %u\n", *data);
    data++;
    return data;
}

char* decode_062_390_CSN(char* data){
    printf("\tCallsign:\n");
    char callsign[8];
    memcpy(callsign, data, 7);
    callsign[7] = '\0';
    printf("\t\t%s\n", callsign);
    data += 7;
    return data;
}

char* decode_062_390_IFI(char* data){
    printf("\tFlight ID:\n");
    unsigned int fid = ((data[0] << 24) & 0x7000000) | (data[1] << 16) & 0xff0000 |
                        (data[2] << 8) & 0xff00 | data[3] & 0xff;
    unsigned char type = data[0] >> 6 & 0x3;
    printf("\t\tType: %u, NBR: %u\n", type, fid);
    data += 4;
    return data;
}

char* decode_062_390_FCT(char* data){
    printf("\tFlight Category:\n");
    printf("\t\tHex data: %#X\n", (unsigned char)data[0]);
    data++;
    return data;
}

char* decode_062_390_TAC(char* data){
    printf("\tType of Aircraft:\n");
    char tac[5];
    memcpy(tac, data, 4);
    tac[4] = '\0';
    printf("\t\t%s\n", tac);
    data += 4;
    return data;
}

char* decode_062_390_WTC(char* data){
    printf("\tWake Turbulence Category:\n");
    printf("\t\t%c\n", data[0]);
    data++;
    return data;
}

char* decode_062_390_DEP(char* data){
    printf("\tDeparture Airport:\n");
    char dep[5];
    memcpy(dep, data, 4);
    dep[4] = '\0';
    printf("\t\t%s\n", dep);
    data += 4;
    return data;
}

char* decode_062_390_DST(char* data){
    printf("\tDestination Airport:\n");
    char des[5];
    memcpy(des, data, 4);
    des[4] = '\0';
    printf("\t\t%s\n", des);
    data += 4;
    return data;
}

char* decode_062_390_RDS(char* data){
    printf("\tRunway Designation:\n");
    char runway[4];
    memcpy(runway, data, 3);
    runway[3] = '\0';
    printf("\t\t%s\n", runway);
    data += 3;
    return data;
}

char* decode_062_390_CFL(char* data){
    printf("\tCurrent cleard flight level:\n");
    short level = ntohs(*(short*)data);
    printf("\t\t%f FL\n", level * 0.25);
    data += 2;
    return data;
}

char* decode_062_390_CTL(char* data){
    printf("\tCurrent Control Position:\n");
    printf("\t\tCentre: %u, Position: %u\n", (unsigned char)data[0], (unsigned char)data[1]);
    data += 2;
    return data;
}

char* decode_062_390_TOD(char* data){
    printf("\tTime of Departure / Arrival:\n");
    char TYP[14][30] = {
      "Scheduled off-block time",
      "Estimated off-block time",
      "Estimated take-off time",
      "Actual off-block time",
      "Predicated time at runway hold",
      "Actual time at runway hold",
      "Actual line-up time",
      "Actual take-off time",
      "Estimated time of arrival",
      "Predicated landing time",
      "Actual landing time",
      "Actual time off runway",
      "Predicated time to gate",
      "Actual on-block time",
    };
    char DAY[4][10] = {
      "Today",
      "Yesterday",
      "Tomorrow",
      "Invalid",
    };

    unsigned char rep = data[0];
    data++;
    for(int i=0; i<rep; i++) {
        printf("\t\tRecord %d:\n", i);
        printf("\t\t\tTYP: %s, DAY: %s, TIME: %02d:%02d",
                  TYP[(unsigned char)data[0] >> 3],
                  DAY[((unsigned char)data[0] >> 1) & 0x3],
                  (unsigned char)data[1] & 0x1f,
                  (unsigned char)data[2] & 0x3f);
        if(!(data[3] & 0x80)) {
            printf(":%02d\n", (unsigned char)data[3] & 0x3f);
        } else {
            printf("\n");
        }
        data += 4;
    }
    return data;
}

char* decode_062_390_AST(char* data){
    printf("\tAircraft Stand:\n");
    char ast[7];
    memcpy(ast, data, 6);
    ast[6] = '\0';
    printf("\t\t%s\n", ast);
    data += 6;
    return data;
}

char* decode_062_390_STS(char* data){
    printf("\tStand status:\n");
    //TODO::
    printf("\t\tEMP: %#x, AVL: %#x\n",
            ((unsigned char)data[0] >> 6) & 0x3,
            ((unsigned char)data[0] >> 4) & 0x3);
    data++;
    return data;
}

char* decode_062_390_STD(char* data){
    printf("\tStandard Instrument Departure:\n");
    char std[8];
    memcpy(std, data, 7);
    std[7] = '\0';
    printf("\t\t%s\n", std);
    data += 7;
    return data;
}

char* decode_062_390_STA(char* data){
    printf("\tStandard Instrument Arrival:\n");
    char sti[8];
    memcpy(sti, data, 7);
    sti[7] = '\0';
    printf("\t\t%s\n", sti);
    data += 7;
    return data;
}

char* decode_062_390_PEM(char* data){
    printf("\tPre-Emergency Mode 3/A:\n");
    printf("\t\tHex data is : 0x%02X%02X\n", (unsigned char)data[0], (unsigned char)data[1]);
    data += 2;
    return data;
}

char* decode_062_390_PEC(char* data){
    printf("\tPre-Emergency Callsign:\n");
    char callsign[8];
    memcpy(callsign, data, 7);
    callsign[7] = '\0';
    printf("\t\t%s\n", callsign);
    data += 7;
    return data;
}

char* decode_062_270(char* data){
    // TODO::
    printf("FIX ME:: I062/270\n");
    return NULL;
}

char* decode_062_300(char* data){
    // TODO::
    printf("I062/300 :\n");
    printf("\tHex data is : 0x%02X\n", (unsigned char)data[0]);
    data++;
    return data;
}

char* decode_062_110(char* data){
    // TODO::
    printf("FIX ME:: I062/110\n");
    return NULL;
}

char* decode_062_120(char* data){
    // TODO::
    printf("I062/120 :\n");
    printf("\tHex data is : 0x%02X%02X\n", (unsigned char)data[0], (unsigned char)data[1]);
    data += 2;
    return data;
}

char* decode_062_510(char* data){
    // TODO::
    printf("FIX ME:: I062/510\n");
    return NULL;
}

char* decode_062_500(char* data){
    // TODO::
    printf("FIX ME:: I062/500\n");
    return NULL;
}

char* decode_062_340(char* data){
    // TODO::
    printf("FIX ME:: I062/340\n");
    return NULL;
}
