#ifndef __DECODE_H__
#define __DECODE_H__

struct __attribute__((packed)) _data {
    int seconds;
    short mseconds;
    short length;
    char logicno;
    char channel;
    char data[0];
}DATA, *PDATA;






#endif
