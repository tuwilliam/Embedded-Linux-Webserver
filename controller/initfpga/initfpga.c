#include "../../common/common.h"
#include "../../common/APP.h"

/*Golbal sqlite3 variable*/
sqlite3* db;
int switchFlag = 0;
int macFlag = 0;
int natFlag = 0;

/*
*If set table switch successful
*@parameter argc is the variable number
*@parameter argv is the variable array(ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, Port_D, UDP, TCP, ARP, ICMP, SIP, NAT)
*@parameter azColusername is the variable name array
*@author SUN ZHOGNJIAN
*/
static int setSwitchCallback(void *NotUsed, int argc, char **argv, char **azColusername){
   
   int i;
   char switchStr[13] = {1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,  0};

   switchFlag = 1;

   if(strcmp(argv[0], "1") == 0){
    switchStr[2] = 0;
   }else if(strcmp(argv[0], "2") == 0){
    switchStr[2] = 128;
   }      
   for(i=1; i<9; i++){
    switchStr[3] |= (atoi(argv[i]) << 8-i);
   }
   for(i=9; i<argc; i++){
    switchStr[4] |= (atoi(argv[i]) << 14-i);
   }
   for(i=0; i<13; i++){
    printf("%d  ", switchStr[i]);
   }


   int fd;
    if((fd=open_port(fd,3))<0){
        printf("open_port error");
        return;
    }
    if((i=set_opt(fd,9600,8,'N',1))<0){
        printf("set_opt error");
        return;
    }
    //printf("fd=%d\n",fd);
    
    write(fd,switchStr,13);
    printf("\n");

    close(fd);
  
   return 0;
}

/*
*Get switch in table switch and send it to FPGA
*@author SUN ZHOGNJIAN
*/
void setSwitch(){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    strcat(sql, "SELECT * FROM  switch;");
    printf("%s  ", sql);
    printf("\n");

    rc = sqlite3_exec(db, sql, setSwitchCallback, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("updateSwitch error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

int setMacRowCallback(void* NotUsed, int argc, char **argv, char **azColusername){

   int i;
   char* stopstring;
   char macRowStr[13] = {1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,  0};

   macFlag = 1;

   for(i = 0; i < 9; i++){
    switch(i){
        case 0: break;
        case 1: macRowStr[i] = 0;
                break;
        case 2: macRowStr[i] = (int)strtol(argv[i], &stopstring, 10);
                break;
        default: macRowStr[i] = (int)strtol(argv[i], &stopstring, 16);
                break;

    }
   }

   int fd;
    if((fd=open_port(fd,3))<0){
        printf("open_port error");
        return;
    }
    if((i=set_opt(fd,9600,8,'N',1))<0){
        printf("set_opt error");
        return;
    }
    //printf("fd=%d\n",fd);

    for(i = 0; i < 13; i++){
        printf("%d ", macRowStr[i]);
    }
    
    write(fd,macRowStr,13);
    printf("\n");

    close(fd);
  
   return 0;
}

void setMacRow(){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(200);
    memset(sql, 0, 200);

    strcat(sql, "SELECT * FROM  mac;");
    printf("%s  ", sql);
    printf("\n");
    
    rc = sqlite3_exec(db, sql, setMacRowCallback, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("set mac error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

int setNatRowCallback(void* NotUsed, int argc, char **argv, char **azColusername){

    natFlag = 1;
    int i;
    char* stopstring;
    char macRowStr[13] = {1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,  0};

    for(i = 0; i < argc; i++){
        switch(i){
            case 0: break;
            case 1: macRowStr[i] = 0;
                    break;
            default: macRowStr[i] = (int)strtol(argv[i], &stopstring, 10);
                    break;

        }
    }

    int fd;
    if((fd=open_port(fd,3))<0){
        printf("open_port error");
        return;
    }
    if((i=set_opt(fd,9600,8,'N',1))<0){
        printf("set_opt error");
        return;
    }

    for(i = 0; i < 13; i++){
        printf("%d ", macRowStr[i]);
    }
    printf("\n");
    write(fd,macRowStr,13);

    close(fd);

    return 0;
}

void setNatRow(){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(200);
    memset(sql, 0, 200);

    strcat(sql, "SELECT * FROM  nat;");
    printf("%s\n", sql);

    rc = sqlite3_exec(db, sql, setNatRowCallback, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("update mac error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

int main(){

    char* zErrMsg = 0;
    int  rc;

    /* Open database */
    rc = sqlite3_open("../model/netgap.db", &db);
    //Error when open DB
    if(rc){
      printf("Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      exit(1);
    }
    setSwitch();
    setMacRow();
    setNatRow();

    while(!macFlag || !switchFlag || !natFlag);
    printf("done\n");
}