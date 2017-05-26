#include "../common/common.h"
#include "../common/APP.h"

int main(int argc, char** argv)
{
  int fd;
  int numLength, i, changePageFlag = 0;

  //printf("Content-type:text/html\n\n");

  FILE *pFile=fopen("cmd.txt","r");
  char fileBuf[100];  
  char IPstr[13] = {1,0};
  char strBuf[10];
  memset(strBuf, 0, 10);

  if((fd=open_port(fd,3))<0){
    printf("open_port error");
    return;
  }
  if((i=set_opt(fd,9600,8,'N',1))<0){
    printf("set_opt error");
    return;
  }
  printf("fd=%d\n",fd);

  
  while (fgets(fileBuf, 100, pFile) != NULL) {
    if (strstr(fileBuf, ",")) {
      for (i = 2; i < 13; i++) {
        numLength = getCharPos(fileBuf, ",", i + 1) - getCharPos(fileBuf, ",", i);
        strncpy(strBuf, getCharPos(fileBuf, ",", i), numLength);
        IPstr[i] = atoi(strBuf);
        memset(strBuf, 0, 10);
      }

      if(changePageFlag)
        IPstr[1] = 1;

      write(fd,IPstr,13);

      for (i = 0; i < 13; i++)
        printf("%d ", IPstr[i]);
      printf("\n");

      if(IPstr[2] == 255){
        printf("change page\n");
        changePageFlag = 1;
      }
    }
    usleep(30000);
  }

      // write(fd,IPstr,13);
   
  printf("done\n");

  close(fd);
  fclose(pFile);

  return 0;
}
