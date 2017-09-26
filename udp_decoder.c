#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include "decode.h"

int main(int argc, char* argv[]) {
  char file_name[100];

  if(argc > 1) {
     strncpy(file_name, argv[1], 5);
     printf("port is %s\n", file_name);

     struct sockaddr_in si;

     int s, i, recv_len;
     char buf[65536];

     //create a UDP socket
     if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
     {
          printf("create socket failed.\n");
          return -1;
     }

     // zero out the structure
     memset((char *) &si, 0, sizeof(si));

     si.sin_family = AF_INET;
     si.sin_port = htons(5566);
     si.sin_addr.s_addr = htonl(INADDR_ANY);

     //bind socket to port
     if(bind(s , (struct sockaddr*)&si, sizeof(si) ) == -1)
     {
         printf("bind failed.\n");
         return -1;
     }

     //keep listening for data
     while(1)
     {
         //try to receive some data, this is a blocking call
         if ((recv_len = recv(s, buf, 65536, 0) == -1))
         {
             printf("recv error.\n");
             return -1;
         }
         if(recv_len) decode((PDATA)buf, recv_len);
     }

     close(s);

  } else {
    printf("Please input port number\n");
  }

  return 0;
}
