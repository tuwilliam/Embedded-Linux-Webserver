#include "uart.h"

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
  struct termios newtio,oldtio;
  if ( tcgetattr( fd,&oldtio) != 0) { 
    printf("SetupSerial 1");
    return -1;
  }
  bzero( &newtio, sizeof( newtio ) );
  newtio.c_cflag |= CLOCAL | CREAD; 
  newtio.c_cflag &= ~CSIZE; 

  switch( nBits )
  {
  case 7:
    newtio.c_cflag |= CS7;
    break;
  case 8:
    newtio.c_cflag |= CS8;
    break;
  }

  switch( nEvent )
  {
  case 'O':
    newtio.c_cflag |= PARENB;
    newtio.c_cflag |= PARODD;
    newtio.c_iflag |= (INPCK | ISTRIP);
    break;
  case 'E': 
    newtio.c_iflag |= (INPCK | ISTRIP);
    newtio.c_cflag |= PARENB;
    newtio.c_cflag &= ~PARODD;
    break;
  case 'N': 
    newtio.c_cflag &= ~PARENB;
    break;
  }

switch( nSpeed )
  {
  case 2400:
    cfsetispeed(&newtio, B2400);
    cfsetospeed(&newtio, B2400);
    break;
  case 4800:
    cfsetispeed(&newtio, B4800);
    cfsetospeed(&newtio, B4800);
    break;
  case 9600:
    cfsetispeed(&newtio, B9600);
    cfsetospeed(&newtio, B9600);
    break;
  case 115200:
    cfsetispeed(&newtio, B115200);
    cfsetospeed(&newtio, B115200);
    break;
  default:
    cfsetispeed(&newtio, B9600);
    cfsetospeed(&newtio, B9600);
    break;
  }
  if( nStop == 1 )
    newtio.c_cflag &= ~CSTOPB;
  else if ( nStop == 2 )
  newtio.c_cflag |= CSTOPB;
  newtio.c_cc[VTIME] = 0;
  newtio.c_cc[VMIN] = 0;
  tcflush(fd,TCIFLUSH);
  if((tcsetattr(fd,TCSANOW,&newtio))!=0)
  {
    printf("com set error");
    return -1;
  }
  //printf("set done!\n");
  return 0;
}

int open_port(int fd,int comport)
{
  char *dev[]={"/dev/ttymxc0","/dev/ttymxc1","/dev/ttymxc2","/dev/ttymxc3","/dev/ttymxc4"};
  long vdisable;
  if (comport==1)
  { fd = open( "/dev/ttymxc0", O_RDWR|O_NOCTTY|O_NDELAY);
    if (-1 == fd){
      printf("Can't Open Serial Port");
      return(-1);
    }
    else ;
      //printf("open ttymxc0 .....\n");
  }
  else if(comport==2)
  { fd = open( "/dev/ttymxc1", O_RDWR|O_NOCTTY|O_NDELAY);
    if (-1 == fd){
      printf("Can't Open Serial Port");
      return(-1);
    }
    else ; 
      //printf("open ttymxc1 .....\n");
  }
  else if (comport==3)
  {
    fd = open( "/dev/ttymxc2", O_RDWR|O_NOCTTY|O_NDELAY);
    if (-1 == fd){
      printf("Can't Open Serial Port");
      return(-1);
    }
    else ;
      //printf("open ttymxc2 .....\n");
  }
  else if (comport==4)
  {
    fd = open( "/dev/ttymxc3", O_RDWR|O_NOCTTY|O_NDELAY);
    if (-1 == fd){
      printf("Can't Open Serial Port");
      return(-1);
    }
    else ;
      //printf("open ttymxc3 .....\n");
  }
  else if (comport==5)
  {
    fd = open( "/dev/ttymxc4", O_RDWR|O_NOCTTY|O_NDELAY);
    if (-1 == fd){
      printf("Can't Open Serial Port");
      return(-1);
    }
    else ;
      //printf("open ttymxc4.....\n");
  }

  if(fcntl(fd, F_SETFL, 0)<0)
    printf("fcntl failed!\n");
  else
    //printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
  if(isatty(STDIN_FILENO)==0)
    printf("standard input is not a terminal device\n");
  else ;
    //printf("isatty success!\n");
  //printf("fd-open=%d\n",fd);
  return fd;
}