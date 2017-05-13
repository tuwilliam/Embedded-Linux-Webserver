#include "userdata.h"

void getjSON(char* input, char* username, char* pwd){

    cJSON *json , *usernameJSON , *pwdJSON; 

    // 
    json = cJSON_Parse(input);  

    if (!json)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
    }  
    else  
    {  
        usernameJSON = cJSON_GetObjectItem( json , "username");  
        strcpy(username,usernameJSON -> valuestring);
        pwdJSON = cJSON_GetObjectItem( json , "pwd");  
		strcpy(pwd,pwdJSON -> valuestring);
        cJSON_Delete(json);  
    }  
}

void makeSession(){
	// char* sql = malloc(1000);
	// strcatsql = "INSERT INTO session (ID,username,pwd,ACCESS) "  \
	//    "VALUES (1, 'root', 'lab555', 0); " \
	//    "INSERT INTO session (ID,username,pwd,ACCESS) "  \
	//    "VALUES (2, 'userLevel1', 'lab555', 1); " \
	//    "INSERT INTO session (ID,username,pwd,ACCESS) "  \
	//    "VALUES (3, 'userLevel2', 'lab555', 2); " \
	//    "INSERT INTO session (ID,username,pwd,ACCESS) "  \
	//    "VALUES (4, 'userLevel3', 'lab555', 3);" \
	//    "INSERT INTO session (ID,username,pwd,ACCESS) "  \
	//    "VALUES (5, 'userLevel4', 'lab555', 4);" \
	//    "INSERT INTO session (ID,username,pwd,ACCESS) "  \
	//    "VALUES (6, 'userLevel5', 'lab555', 5);";
	// char* zErrMsg = 0;

	// /* Execute SQL statement */
	// rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	// if( rc != SQLITE_OK ){
	// fprintf(stderr, "SQL error: %s\n", zErrMsg);
	// sqlite3_free(zErrMsg);
	// }else{
	// fprintf(stdout, "Records Insert successfully\n");
	// }
	// free(sql);

}


char* makeJSON()
{
    cJSON* pJSON = NULL;

    pJSON = cJSON_CreateObject();
    if(pJSON == NULL)
    {
        //error happend here
        return NULL;
    }
    cJSON_AddStringToObject(pJSON, "status", "success");
    cJSON_AddStringToObject(pJSON, "token", "whatever");

    char* p = cJSON_Print(pJSON);
  // else use : 
    // char * p = cJSON_PrintUnformatted(pJSON);
    if(p == NULL)
    {
        //convert json list to string faild, exit
        //because sub json pSubJson han been add to pJSON, so just delete pJSON, if you also delete pSubJson, it will coredump, and error is : double free
        cJSON_Delete(pJSON);
        return NULL;
    }
    //free(p);
    
    cJSON_Delete(pJSON);

    return p;
}