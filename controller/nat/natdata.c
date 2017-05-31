#include "../../common/common.h"
#include "../../common/APP.h"


/*
*Find mac flag in table switch
*@author SUN ZHOGNJIAN
*/
void getNatSwitch(){

    int  rc;
    char* sql = malloc(100);
    memset(sql, 0, 100);
    strcat(sql, "SELECT * FROM  switch;");
    char* zErrMsg = 0;

    rc = sqlite3_exec(db, sql, getSwitchCallback, NAT_Flag, &zErrMsg);
    //When sql format is incorrect
    if( rc != SQLITE_OK ){
      printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    free(sql);
}

/*
*If set table switch successful
*@parameter argc is the variable number
*@parameter argv is the variable array(ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, Port_D, UDP, TCP, ARP, ICMP, SIP, NAT)
*@parameter azColusername is the variable name array
*@author SUN ZHOGNJIAN
*/
static int setNatSwitchCallback(void *NotUsed, int argc, char **argv, char **azColusername){
   
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
void setNatSwitch(char* state){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    //update switch according to row ID
    strcat(sql, "UPDATE switch SET NAT=\'");
    if(strcmp(state, "true")==0){
        strcat(sql, "1");
    }else if(strcmp(state, "false")==0){
        strcat(sql, "0");
    }
    
    strcat(sql, "\' WHERE ID IN (1,2);");
    strcat(sql, "SELECT * FROM  switch;");
    printf("%s   ", sql);
    rc = sqlite3_exec(db, sql, setNatSwitchCallback, 0, &zErrMsg);
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
cJSON* makeNatJSON(char* id, char* eth , char* addr, char* ipsStr, char* ipdStr){
    cJSON* pJSON = NULL;

    pJSON = cJSON_CreateObject();
    if(pJSON == NULL){
        //Error happend here
        return NULL;
    }
    cJSON_AddStringToObject(pJSON, "id", id);
    cJSON_AddStringToObject(pJSON, "eth", eth);
    cJSON_AddStringToObject(pJSON, "addr", addr);
    cJSON_AddStringToObject(pJSON, "ips", ipsStr);
    cJSON_AddStringToObject(pJSON, "ipd", ipdStr);

    return pJSON;
}


cJSON* objects[16];
int jSON_Index = 0;

/*
*If find flag in table switch
*@parameter switchFlag is the flag index, check it in variable array
*@parameter argc is the variable number
*@parameter argv is the variable array(ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, Port_D, UDP, TCP, ARP, ICMP, SIP, NAT)
*@parameter azColusername is the variable name array
*@author SUN ZHOGNJIAN
*/
int getNatCallback(void *Notused, int argc, char **argv, char **azColName){   

    int i;
    char ipsStr[16];
    char ipdStr[16];

    memset(ipsStr, 0 ,18);
    memset(ipdStr, 0 ,18);

    for(i = 3; i < 6; i++){
        strcat(ipsStr, argv[i]);
        strcat(ipsStr, ".");
    }
    strcat(ipsStr, argv[6]);

    for(i = 7; i < 10; i++){
        strcat(ipdStr, argv[i]);
        strcat(ipdStr, ".");
    }
    strcat(ipdStr, argv[10]);

    objects[jSON_Index++] = makeNatJSON(argv[0], argv[1], argv[2], ipsStr, ipdStr);

    if(jSON_Index == 16){
        int i;
        cJSON *prev;
        cJSON *root = cJSON_CreateArray();
        for (i = 0; i < 16; i++)
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

void getNat(){

    int  rc;
    char* sql = malloc(100);
    memset(sql, 0, 100);
    strcat(sql, "SELECT * FROM nat limit 0,8; SELECT * FROM nat limit 16,8;");

    char* zErrMsg = 0;

    rc = sqlite3_exec(db, sql, getNatCallback, 0, &zErrMsg);
    //When sql format is incorrect
    if( rc != SQLITE_OK ){
      printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    free(sql);
}

void getNatJSON(char* input, char* id, char* eth, char* addr, char* SIP_3, char* SIP_2, char* SIP_1, char* SIP_0, char* DIP_3, char* DIP_2, char* DIP_1, char* DIP_0){
    cJSON *json , *idJSON , *ethJSON , *addrJSON , *SIP_3JSON , *SIP_2JSON , *SIP_1JSON , *SIP_0JSON , *DIP_3JSON , *DIP_2JSON, *DIP_1JSON, *DIP_0JSON; 
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
        SIP_3JSON = cJSON_GetObjectItem( json , "SIP_3");  
        strcpy(SIP_3,SIP_3JSON -> valuestring);
        SIP_2JSON = cJSON_GetObjectItem( json , "SIP_2");  
        strcpy(SIP_2,SIP_2JSON -> valuestring);
        SIP_1JSON = cJSON_GetObjectItem( json , "SIP_1");  
        strcpy(SIP_1,SIP_1JSON -> valuestring);
        SIP_0JSON = cJSON_GetObjectItem( json , "SIP_0");  
        strcpy(SIP_0,SIP_0JSON -> valuestring);
        DIP_3JSON = cJSON_GetObjectItem( json , "DIP_3");  
        strcpy(DIP_3,DIP_3JSON -> valuestring);
        DIP_2JSON = cJSON_GetObjectItem( json , "DIP_2"); 
        strcpy(DIP_2,DIP_2JSON -> valuestring);
        DIP_1JSON = cJSON_GetObjectItem( json , "DIP_1");
        strcpy(DIP_1,DIP_1JSON -> valuestring);
        DIP_0JSON = cJSON_GetObjectItem( json , "DIP_0"); 
        strcpy(DIP_0,DIP_0JSON -> valuestring);
        cJSON_Delete(json);  
    }  
}

int setNatRowCallback(void* NotUsed, int argc, char **argv, char **azColusername){
    printf("setMacRowCallback   ");
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
    printf("fd=%d\n",fd);

    for(i = 0; i < 13; i++){
        printf("%d ", macRowStr[i]);
    }

    write(fd,macRowStr,13);

    close(fd);

    return 0;
}

void setNatRow(char* id, char* eth, char* addr, char* SIP_3, char* SIP_2, char* SIP_1, char* SIP_0, char* DIP_3, char* DIP_2, char* DIP_1, char* DIP_0){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(200);
    memset(sql, 0, 200);

    //update switch according to row ID
    strcat(sql, "UPDATE nat SET SIP_3=\'");
    strcat(sql, SIP_3);
    strcat(sql, "\' , SIP_2=\'");
    strcat(sql, SIP_2);
    strcat(sql, "\' , SIP_1=\'");
    strcat(sql, SIP_1);
    strcat(sql, "\' , SIP_0=\'");
    strcat(sql, SIP_0);
    strcat(sql, "\' , DIP_3=\'");
    strcat(sql, DIP_3);
    strcat(sql, "\' , DIP_2=\'");
    strcat(sql, DIP_2);
    strcat(sql, "\' , DIP_1=\'");
    strcat(sql, DIP_1);
    strcat(sql, "\' , DIP_0=\'");
    strcat(sql, DIP_0);
    strcat(sql, "\' WHERE addr=\'");
    strcat(sql, addr);
    strcat(sql, "\';");
    strcat(sql, "SELECT * FROM  nat WHERE addr = \'");
    strcat(sql, addr);
    strcat(sql, "\';");

    printf("%s  ", sql);

    rc = sqlite3_exec(db, sql, setNatRowCallback, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("update nat error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

