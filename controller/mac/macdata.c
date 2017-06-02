#include "../../common/common.h"
#include "../../common/APP.h"


/*
*Find mac flag in table switch
*@author SUN ZHOGNJIAN
*/
void getMacSwitch(){

    int  rc;
    char* sql = malloc(100);
    memset(sql, 0, 100);
    strcat(sql, "SELECT * FROM  switch;");
    char* zErrMsg = 0;

    rc = sqlite3_exec(db, sql, getSwitchCallback, MAC_SA_Flag, &zErrMsg);
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
static int setMacSwitchCallback(void *NotUsed, int argc, char **argv, char **azColusername){
   
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
    printf("fd=%d\n",fd);

    write(fd,switchStr,13);

    close(fd);
  
   return 0;
}

/*
*Set mac flag in table switch
*@author SUN ZHOGNJIAN
*/
void setMacSwitch(char* row, char* state){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    //update switch according to row ID
    strcat(sql, "UPDATE switch SET MAC_SA=\'");
    if(strcmp(state, "true")==0){
        strcat(sql, "1");
    }else if(strcmp(state, "false")==0){
        strcat(sql, "0");
    }
    
    strcat(sql, "\' WHERE ID=");
    strcat(sql, row);
    strcat(sql, ";");
    strcat(sql, "SELECT * FROM  switch;");

    rc = sqlite3_exec(db, sql, setMacSwitchCallback, 0, &zErrMsg);
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
cJSON* makeMacJSON(char* id, char* eth , char* addr, char* mac){
    cJSON* pJSON = NULL;

    pJSON = cJSON_CreateObject();
    if(pJSON == NULL){
        //Error happend here
        return NULL;
    }
    cJSON_AddStringToObject(pJSON, "id", id);
    cJSON_AddStringToObject(pJSON, "eth", eth);
    cJSON_AddStringToObject(pJSON, "addr", addr);
    cJSON_AddStringToObject(pJSON, "mac", mac);

    return pJSON;
}


cJSON* objects[32];
int jSON_Index = 0;

/*
*If find flag in table switch
*@parameter switchFlag is the flag index, check it in variable array
*@parameter argc is the variable number
*@parameter argv is the variable array(ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, Port_D, UDP, TCP, ARP, ICMP, SIP, NAT)
*@parameter azColusername is the variable name array
*@author SUN ZHOGNJIAN
*/
int getMacCallback(void *Notused, int argc, char **argv, char **azColName){   

    int i;
    char macStr[18];
    memset(macStr, 0 ,18);
    for(i = 0; i < 5; i++){
        strcat(macStr, argv[i+3]);
        strcat(macStr, "-");
    }
    strcat(macStr, argv[8]);
    objects[jSON_Index++] = makeMacJSON(argv[0], argv[1], argv[2], macStr);

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

void getMac(){

    int  rc;
    char* sql = malloc(100);
    memset(sql, 0, 100);
    strcat(sql, "SELECT * FROM  mac;");
    char* zErrMsg = 0;

    rc = sqlite3_exec(db, sql, getMacCallback, 0, &zErrMsg);
    //When sql format is incorrect
    if( rc != SQLITE_OK ){
      printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    free(sql);
}


void getMacJSON(char* input, char* id, char* eth, char* addr, char* mac_5, char* mac_4, char* mac_3, char* mac_2, char* mac_1, char* mac_0){
    cJSON *json , *idJSON , *ethJSON , *addrJSON , *mac_5JSON , *mac_4JSON , *mac_3JSON , *mac_2JSON , *mac_1JSON , *mac_0JSON; 
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
        mac_5JSON = cJSON_GetObjectItem( json , "mac_5");  
        strcpy(mac_5,mac_5JSON -> valuestring);
        mac_4JSON = cJSON_GetObjectItem( json , "mac_4");  
        strcpy(mac_4,mac_4JSON -> valuestring);
        mac_3JSON = cJSON_GetObjectItem( json , "mac_3");  
        strcpy(mac_3,mac_3JSON -> valuestring);
        mac_2JSON = cJSON_GetObjectItem( json , "mac_2");  
        strcpy(mac_2,mac_2JSON -> valuestring);
        mac_1JSON = cJSON_GetObjectItem( json , "mac_1");  
        strcpy(mac_1,mac_1JSON -> valuestring);
        mac_0JSON = cJSON_GetObjectItem( json , "mac_0");  
        strcpy(mac_0,mac_0JSON -> valuestring);
        cJSON_Delete(json);  
    }  
}

int setMacRowCallback(void* NotUsed, int argc, char **argv, char **azColusername){

   int i;
   char* stopstring;
   char macRowStr[13] = {1, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,  0};

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
    printf("fd=%d\n",fd);

    for(i = 0; i < 13; i++){
        printf("%d ", macRowStr[i]);
    }

    write(fd,macRowStr,13);

    close(fd);
  
   return 0;
}

void setMacRow(char* id, char* eth, char* addr, char* mac_5, char* mac_4, char* mac_3, char* mac_2, char* mac_1, char* mac_0){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(200);
    memset(sql, 0, 200);

    //update switch according to row ID
    strcat(sql, "UPDATE mac SET mac_5=\'");
    strcat(sql, mac_5);
    strcat(sql, "\' , mac_4=\'");
    strcat(sql, mac_4);
    strcat(sql, "\' , mac_3=\'");
    strcat(sql, mac_3);
    strcat(sql, "\' , mac_2=\'");
    strcat(sql, mac_2);
    strcat(sql, "\' , mac_1=\'");
    strcat(sql, mac_1);
    strcat(sql, "\' , mac_0=\'");
    strcat(sql, mac_0);
    strcat(sql, "\' WHERE addr=\'");
    strcat(sql, addr);
    strcat(sql, "\';");
    strcat(sql, "SELECT * FROM  mac WHERE addr = \'");
    strcat(sql, addr);
    strcat(sql, "\';");

    printf("%s  ", sql);

    rc = sqlite3_exec(db, sql, setMacRowCallback, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("update mac error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

