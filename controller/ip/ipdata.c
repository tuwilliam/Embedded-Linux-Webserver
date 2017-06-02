#include "../../common/common.h"
#include "../../common/APP.h"


/*
*Find mac flag in table switch
*@author SUN ZHOGNJIAN
*/
void getIpSwitch(){

    int  rc;
    char* sql = malloc(100);
    memset(sql, 0, 100);
    strcat(sql, "SELECT * FROM  switch;");
    char* zErrMsg = 0;

    rc = sqlite3_exec(db, sql, getSwitchCallback, IP_SA_Flag, &zErrMsg);
    //When sql format is incorrect
    if( rc != SQLITE_OK ){
      printf("get switch error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    rc = sqlite3_exec(db, sql, getSwitchCallback, IP_DA_Flag, &zErrMsg);
    //When sql format is incorrect
    if( rc != SQLITE_OK ){
      printf("get switch error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    free(sql);
}

/*
*If set table switch successful
*@parameter argc is the variable number
*@parameter argv is the variable array(ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, Port_D, UDP, TCP, ARP, ICMP, IP, Ip)
*@parameter azColusername is the variable name array
*@author SUN ZHOGNJIAN
*/
static int setIpSwitchCallback(void *NotUsed, int argc, char **argv, char **azColusername){
   
   int i;
   char switchStr[13] = {1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,  0};

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

    write(fd,switchStr,13);

    close(fd);
  
   return 0;
}

/*
*Set mac flag in table switch
*@author SUN ZHOGNJIAN
*/
void setIpSwitch(char* field, char* state){
    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    //update switch according to row ID
    strcat(sql, "UPDATE switch SET ");
    if(strcmp(field, "inner") == 0){
        strcat(sql, "IP_SA=\'");
    }else if(strcmp(field, "outer") == 0){
        strcat(sql, "IP_DA=\'");
    }

    if(strcmp(state, "true") == 0){
        strcat(sql, "1");
    }else if(strcmp(state, "false")==0){
        strcat(sql, "0");
    }
    
    strcat(sql, "\' WHERE ID=");

    if(strcmp(field, "inner") == 0){
        strcat(sql, "1;");
    }else if(strcmp(field, "outer") == 0){
        strcat(sql, "2;");
    }

    strcat(sql, "SELECT * FROM  switch;");
    printf("sql = %s", sql);
    rc = sqlite3_exec(db, sql, setIpSwitchCallback, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("updateSwitch error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}



/*
*Set mac flag in table switch
*@author SUN ZHOGNJIAN
*/
cJSON* makeIpJSON(char* id, char* eth , char* addr, char* ipStr){
    cJSON* pJSON = NULL;

    pJSON = cJSON_CreateObject();
    if(pJSON == NULL){
        //Error happend here
        return NULL;
    }
    cJSON_AddStringToObject(pJSON, "id", id);
    cJSON_AddStringToObject(pJSON, "eth", eth);
    cJSON_AddStringToObject(pJSON, "addr", addr);
    cJSON_AddStringToObject(pJSON, "ip", ipStr);

    return pJSON;
}


cJSON* objects[32];
int jSON_Index = 0;

/*
*If find flag in table switch
*@parameter switchFlag is the flag index, check it in variable array
*@parameter argc is the variable number
*@parameter argv is the variable array(ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, Port_D, UDP, TCP, ARP, ICMP, IP, Ip)
*@parameter azColusername is the variable name array
*@author SUN ZHOGNJIAN
*/
int getIpCallback(void *Notused, int argc, char **argv, char **azColName){   

    int i;
    char ipStr[16];

    memset(ipStr, 0 ,18);

    for(i = 0; i < 7; i++){
        switch(i){
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: strcat(ipStr, argv[i]); strcat(ipStr, "."); break;
        case 4: strcat(ipStr, argv[i]); strcat(ipStr, "."); break;
        case 5: strcat(ipStr, argv[i]); strcat(ipStr, "."); break;
        case 6: strcat(ipStr, argv[i]); break;
        }        
    }

    objects[jSON_Index++] = makeIpJSON(argv[0], argv[1], argv[2], ipStr);

    if(jSON_Index == 32){
        int i;
        cJSON *prev;
        cJSON *root = cJSON_CreateArray();
        for (i = 0; i < 32; i++)
        {
            if (!i)
            {
                root -> child = objects[i];
            }
            else
            {
                prev -> next = objects[i];
                objects[i] -> prev = prev;
            }

            prev = objects[i];
        }
        printf("%s   \n", cJSON_Print(root));
    }
    return 0;
}

void getIp(){

    int  rc;
    char* sql = malloc(100);
    memset(sql, 0, 100);
    strcat(sql, "SELECT * FROM ip;");

    char* zErrMsg = 0;

    rc = sqlite3_exec(db, sql, getIpCallback, 0, &zErrMsg);
    //When sql format is incorrect
    if( rc != SQLITE_OK ){
      printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    free(sql);
}

void getIpJSON(char* input, char* id, char* eth, char* addr, char* IP_3, char* IP_2, char* IP_1, char* IP_0){
    cJSON *json , *idJSON , *ethJSON , *addrJSON , *IP_3JSON , *IP_2JSON , *IP_1JSON , *IP_0JSON; 
    json = cJSON_Parse(input);  

    if (!json){  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
    }  
    else{  
        idJSON = cJSON_GetObjectItem( json , "id");  
        strcpy(id,idJSON -> valuestring);
        printf("getJSON id = %s     ", id);
        ethJSON = cJSON_GetObjectItem( json , "eth");  
        strcpy(eth,ethJSON -> valuestring);
        addrJSON = cJSON_GetObjectItem( json , "addr");  
        strcpy(addr,addrJSON -> valuestring);
        IP_3JSON = cJSON_GetObjectItem( json , "IP_3");  
        strcpy(IP_3,IP_3JSON -> valuestring);
        IP_2JSON = cJSON_GetObjectItem( json , "IP_2");  
        strcpy(IP_2,IP_2JSON -> valuestring);
        IP_1JSON = cJSON_GetObjectItem( json , "IP_1");  
        strcpy(IP_1,IP_1JSON -> valuestring);
        IP_0JSON = cJSON_GetObjectItem( json , "IP_0");  
        strcpy(IP_0,IP_0JSON -> valuestring);
        cJSON_Delete(json);  
    }  
}

int setIpRowCallback(void* NotUsed, int argc, char **argv, char **azColusername){

    int i;
    char* stopstring;
    char IpRowStr[13] = {1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,  0};
    

    for(i = 0; i < argc; i++){
        switch(i){
            case 0: break;
            case 1: IpRowStr[i] = 0;
                    break;
            default: IpRowStr[i] = (int)strtol(argv[i], &stopstring, 10);
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
    printf("fd=%d\n",fd);

    for(i = 0; i < 13; i++){
        printf("%d ", IpRowStr[i]);
    }

    write(fd,IpRowStr,13);




    
    usleep(30000);
    


    int nread = 0;
    int ret;
    struct pollfd event;
    
    char readBuf[10];
    memset(readBuf, 0, 10);

    memset(&event,0,sizeof(event));
    event.fd = fd;
    event.events = POLLIN;
    ret=poll((struct pollfd *)&event,1,50);

    if(ret<0){
        printf("poll error!\n");
        exit(1);
    }
    if(ret==0){
        printf("Time out!\n");
    }
    if(event.revents&POLLERR){ //revents是由内核记录的实际发生的事件，events是进程等待的事件
        printf("Device error!\n");
        exit(1);
    }
    if(event.revents&POLLIN){
        nread = read(fd,readBuf,10);
        printf("nread = %d,%s\n",nread,readBuf);
    }
    


    close(fd);

    return 0;
}

void setIpRow(char* id, char* eth, char* addr, char* IP_3, char* IP_2, char* IP_1, char* IP_0){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(200);
    memset(sql, 0, 200);

    //update switch according to row ID
    strcat(sql, "UPDATE ip SET IP_3=\'");
    strcat(sql, IP_3);
    strcat(sql, "\' , IP_2=\'");
    strcat(sql, IP_2);
    strcat(sql, "\' , IP_1=\'");
    strcat(sql, IP_1);
    strcat(sql, "\' , IP_0=\'");
    strcat(sql, IP_0);
    strcat(sql, "\' WHERE addr=\'");
    strcat(sql, addr);
    strcat(sql, "\';");
    strcat(sql, "SELECT * FROM  ip WHERE addr = \'");
    strcat(sql, addr);
    strcat(sql, "\';");

    printf("%s  ", sql);

    rc = sqlite3_exec(db, sql, setIpRowCallback, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("update Ip error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

