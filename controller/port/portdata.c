#include "../../common/common.h"
#include "../../common/APP.h"


/*
*Find mac flag in table switch
*@author SUN ZHOGNJIAN
*/
void getPortSwitch(){

    int  rc;
    char* sql = malloc(100);
    memset(sql, 0, 100);
    strcat(sql, "SELECT * FROM  switch;");
    char* zErrMsg = 0;

    rc = sqlite3_exec(db, sql, getSwitchCallback, Port_S_Flag, &zErrMsg);
    //When sql format is incorrect
    if( rc != SQLITE_OK ){
      printf("get switch error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    rc = sqlite3_exec(db, sql, getSwitchCallback, Port_D_Flag, &zErrMsg);
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
*@parameter argv is the variable array(ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, Port_D, UDP, TCP, ARP, ICMP, SIP, Port)
*@parameter azColusername is the variable name array
*@author SUN ZHOGNJIAN
*/
static int setPortSwitchCallback(void *NotUsed, int argc, char **argv, char **azColusername){
   
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
void setPortSwitch(char* field, char* state){
    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    //update switch according to row ID
    strcat(sql, "UPDATE switch SET ");
    if(strcmp(field, "inner") == 0){
        strcat(sql, "Port_S=\'");
    }else if(strcmp(field, "outer") == 0){
        strcat(sql, "Port_D=\'");
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
    rc = sqlite3_exec(db, sql, setPortSwitchCallback, 0, &zErrMsg);
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
cJSON* makePortJSON(char* id, char* eth , char* addr, char* portStr, char* protocolStr){
    cJSON* pJSON = NULL;

    pJSON = cJSON_CreateObject();
    if(pJSON == NULL){
        //Error happend here
        return NULL;
    }
    cJSON_AddStringToObject(pJSON, "id", id);
    cJSON_AddStringToObject(pJSON, "eth", eth);
    cJSON_AddStringToObject(pJSON, "addr", addr);
    cJSON_AddStringToObject(pJSON, "port", portStr);
    cJSON_AddStringToObject(pJSON, "protocol", protocolStr);

    return pJSON;
}


cJSON* objects[32];
int jSON_Index = 0;

/*
*If find flag in table switch
*@parameter switchFlag is the flag index, check it in variable array
*@parameter argc is the variable number
*@parameter argv is the variable array(ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, Port_D, UDP, TCP, ARP, ICMP, SIP, Port)
*@parameter azColusername is the variable name array
*@author SUN ZHOGNJIAN
*/
int getPortCallback(void *Notused, int argc, char **argv, char **azColName){   
    
    int i;
    char portStr[6];
    char protocolStr[13];
    memset(portStr, 0 ,6);
    memset(protocolStr, 0 ,13);

    sprintf(portStr, "%d", atoi(argv[3]) * 256 + atoi(argv[4]));

    switch(atoi(argv[5])){
        case 0: strcat(protocolStr, "Both Banned");   break;
        case 1: strcat(protocolStr, "TCP");           break;
        case 2: strcat(protocolStr, "UDP");           break;
        case 3: strcat(protocolStr, "Both Allowed");  break;
    }

    objects[jSON_Index++] = makePortJSON(argv[0], argv[1], argv[2], portStr, protocolStr);

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

void getPort(){
    int  rc;
    char* sql = malloc(100);
    memset(sql, 0, 100);
    strcat(sql, "SELECT * FROM port limit 0,16; SELECT * FROM port limit 48,16;");
    
    char* zErrMsg = 0;

    rc = sqlite3_exec(db, sql, getPortCallback, 0, &zErrMsg);
    //When sql format is incorrect
    if( rc != SQLITE_OK ){
      printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    free(sql);
}

void getPortJSON(char* input, char* id, char* eth, char* addr, char* port_h, char* port_l, char* protocol){
    cJSON *json , *idJSON , *ethJSON , *addrJSON , *port_hJSON , *port_lJSON , *protocolJSON; 
    json = cJSON_Parse(input);  

    if (!json){  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
    }  
    else{  
        idJSON = cJSON_GetObjectItem( json , "id");  
        strcpy(id,idJSON -> valuestring);
        //printf("getJSON id = %s     ", id);
        ethJSON = cJSON_GetObjectItem( json , "eth");  
        strcpy(eth,ethJSON -> valuestring);
        //printf("getJSON eth = %s     ", eth);
        addrJSON = cJSON_GetObjectItem( json , "addr");  
        strcpy(addr,addrJSON -> valuestring);
        //printf("getJSON addr = %s     ", addr);
        port_hJSON = cJSON_GetObjectItem( json , "port_h");  
        strcpy(port_h, port_hJSON -> valuestring);
        //printf("getJSON port_h = %s     ", port_h);
        port_lJSON = cJSON_GetObjectItem( json , "port_l");  
        strcpy(port_l,port_lJSON -> valuestring);
        //printf("getJSON port_l = %s     ", port_l);
        protocolJSON = cJSON_GetObjectItem( json , "protocol");  
        strcpy(protocol,protocolJSON -> valuestring);
        //printf("getJSON protocol = %s     ", protocol);
        cJSON_Delete(json);  
    }  
}

int setPortRowCallback(void* NotUsed, int argc, char **argv, char **azColusername){

    int i;
    char* stopstring;
    char portRowStr[13] = {1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,  0};

    for(i = 0; i < argc; i++){
        switch(i){
            case 0: break;
            case 1: portRowStr[i] = 0;
                    break;
            default: portRowStr[i] = (int)strtol(argv[i], &stopstring, 10);
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
        printf("%d ", portRowStr[i]);
    }

    write(fd, portRowStr,13);

    close(fd);

    return 0;
}

void setPortRow(char* id, char* eth, char* addr, char* PORT_H, char* PORT_L, char* PROTOCOL){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(200);
    memset(sql, 0, 200);

    //update switch according to row ID
    strcat(sql, "UPDATE port SET PORT_H=\'");
    strcat(sql, PORT_H);
    strcat(sql, "\' , PORT_L=\'");
    strcat(sql, PORT_L);
    strcat(sql, "\' , PROTOCOL=\'");
    strcat(sql, PROTOCOL);
    strcat(sql, "\' WHERE addr=\'");
    strcat(sql, addr);
    strcat(sql, "\';");
    strcat(sql, "SELECT * FROM  Port WHERE addr = \'");
    strcat(sql, addr);
    strcat(sql, "\';");

    //printf("%s  ", sql);

    rc = sqlite3_exec(db, sql, setPortRowCallback, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("update Port error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

