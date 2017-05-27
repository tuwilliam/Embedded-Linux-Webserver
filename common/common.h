/*
  Every library is here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "sqlite3.h"
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <regex.h>

/* Returns the string that web front-end to web back-end */
char* getCgiData(FILE* fp, char* method);
/* Returns a string's position at the Nth time occurrence of the source string */
char* getCharPos(char* scr, char* search, int pos);
/*If find flag in table switch*/
int getSwitchCallback(void *switchFlag, int argc, char **argv, char **azColName);

//Switch flags define
#define MAC_DA_Flag         "1"
#define MAC_SA_Flag         "2"
#define IPv4_Flag           "3"
#define IP_PROTOCOL_Flag    "4"
#define IP_SA_Flag          "5"
#define IP_DA_Flag          "6"
#define Port_S_Flag         "7"
#define Port_D_Flag         "8"
#define UDP_Flag            "9"
#define TCP_Flag            "10"
#define ARP_Flag            "11"
#define ICMP_Flag           "12"
#define SIP_Flag            "13"
#define NAT_Flag            "14"

#define innerSwitch_Flag    "1"
#define outerSwitch_Flag    "2"