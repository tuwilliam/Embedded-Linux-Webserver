#include "../../common/common.h"
#include "../../common/APP.h"

void getMacSwitch(){

    int numLength, i;
    FILE *pFile=fopen("cmd.txt","r");
    char fileBuf[100];  
    char IPstr[13] = {1};
    char strBuf[10];
    memset(strBuf, 0, 10);

    while (fgets(fileBuf, 100, pFile) != NULL) {
        if (strstr(fileBuf, ",")) {
          for (i = 1; i < 13; i++) {
            numLength = getCharPos(fileBuf, ",", i + 1) - getCharPos(fileBuf, ",", i);
            strncpy(strBuf, getCharPos(fileBuf, ",", i), numLength);
            IPstr[i] = atoi(strBuf);
            memset(strBuf, 0, 10);
          }

          if(IPstr[1] == 0 && IPstr[2] == 0){
            printf("%d,", (IPstr[3] & 0x40) == 0x40);
          }

          if(IPstr[1] == 0 && IPstr[2] == 128){
            printf("%d", (IPstr[3] & 0x40) == 0x40);
          }
        }
    }

    fclose(pFile);
}

void setMacSwitch(int row, char* state){

    int fd, i;
    if((fd=open_port(fd,3))<0){
    printf("open_port error");
    return;
    }
    if((i=set_opt(fd,9600,8,'N',1))<0){
    printf("set_opt error");
    return;
    }

    int numLength;
    FILE *pFile=fopen("cmd.txt","rw");
    char fileBuf[100];  
    char IPstr[13] = {1};
    char strBuf[10];
    memset(strBuf, 0, 10);

    while (fgets(fileBuf, 100, pFile) != NULL) {
        if (strstr(fileBuf, ",")) {
          for (i = 1; i < 13; i++) {
            numLength = getCharPos(fileBuf, ",", i + 1) - getCharPos(fileBuf, ",", i);
            strncpy(strBuf, getCharPos(fileBuf, ",", i), numLength);
            IPstr[i] = atoi(strBuf);
            memset(strBuf, 0, 10);
          }

          if(IPstr[1] == 0 && IPstr[2] == row){
            if(strcmp(state, "true") == 0){
                IPstr[3] |= 0x40;
            }else if(strcmp(state, "false") == 0){
                IPstr[3] &= 0xBF;
            }

            write(fd,IPstr,13);

            for (i = 0; i < 13; i++){
                printf("%d ", IPstr[i]);
            }
          }
        }
    }
    close(fd);
}

int main(int argc, char** argv)
{
    char* method;
    char* uri;
    printf("Content-type:text/html\n\n");
    //Get request method
    method = getenv("REQUEST_METHOD");  

    //Get request uri  
    uri = getenv("REQUEST_URI");
    char* subRouter = getCharPos(uri, "/", 4);
    
    if(strcmp(subRouter, "switch") == 0 && strcmp(method, "GET") == 0){
        getMacSwitch();
    }

    if(strcmp(subRouter, "innerswitch/true") == 0 && strcmp(method, "POST") == 0){
        setMacSwitch(0, getCharPos(uri, "/", 5));
    }

    if(strcmp(subRouter, "innerswitch/false") == 0 && strcmp(method, "POST") == 0){
        setMacSwitch(0, getCharPos(uri, "/", 5));
    }

  return 0;
}