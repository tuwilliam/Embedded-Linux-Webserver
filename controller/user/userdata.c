#include "userdata.h"

/*Flag indicates if table session have the user_access_id*/
char sessionCheckFlag = 0;

void getjSON(char* input, char* username, char* pwd){
    cJSON *json , *usernameJSON , *pwdJSON; 
    json = cJSON_Parse(input);  

    if (!json){  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
    }  
    else{  
        usernameJSON = cJSON_GetObjectItem( json , "username");  
        strcpy(username,usernameJSON -> valuestring);
        pwdJSON = cJSON_GetObjectItem( json , "pwd");  
		strcpy(pwd,pwdJSON -> valuestring);
        cJSON_Delete(json);  
    }  
}

char* makeJSON(char* status, char* tokenStr , char* user_access_id)
{
    cJSON* pJSON = NULL;

    pJSON = cJSON_CreateObject();
    if(pJSON == NULL)
    {
        //error happend here
        return NULL;
    }
    cJSON_AddStringToObject(pJSON, "status", status);
    cJSON_AddStringToObject(pJSON, "token", tokenStr);
    cJSON_AddStringToObject(pJSON, "id", user_access_id);

    char* p = cJSON_Print(pJSON);
    if(p == NULL)
    {
        //convert json list to string faild, exit
        //because sub json pSubJson han been add to pJSON, so just delete pJSON, if you also delete pSubJson, it will coredump, and error is : double free
        cJSON_Delete(pJSON);
        return NULL;
    }
    
    cJSON_Delete(pJSON);

    return p;
}

void makeToken(char* tokenStr)
{
    srand((unsigned) time(NULL));
    sprintf(tokenStr, "%d", rand() % 100000000);//1_0000_0000
}

/*
*If find user_access_id in table session, enter callback, update session ID
*@parameter pwd is the token string need to be updated
*@parameter argc is the variable number
*@parameter argv is the variable array(session_id, token, user_access_id, time)
*@parameter azColusername is the variable name array("ID", "token", "user_access_id","time")
*@author SUN ZHOGNJIAN
*/
static int checkUserSessionCallback(void *tokenStr, int argc, char **argv, char **azColusername)
{
    sessionCheckFlag = 1;
    updateSession(argv[0], (char*)tokenStr, db);
    printf("%s\n", makeJSON("success", (char*)tokenStr, argv[2]));
    return 0;
}

/*
*check if user_access_id is in table session ? (logged)update session : (never logged)make session
*@parameter user_access_id is user_access_id
*@parameter db is the netgap database
*@author SUN ZHOGNJIAN
*/
void checkUserSession(char* user_access_id, sqlite3* db)
{
    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    char tokenStr[8];
    makeToken(tokenStr);

    strcat(sql, "SELECT * FROM session WHERE user_access_id=");
    strcat(sql, user_access_id);
    strcat(sql, ";");

    rc = sqlite3_exec(db, sql, checkUserSessionCallback, tokenStr, &zErrMsg);
    
    if( rc != SQLITE_OK ){
      printf("session Table error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    if(!sessionCheckFlag){
        makeSession(user_access_id, tokenStr, db);
        printf("%s\n", makeJSON("success", tokenStr, user_access_id));
    }
    else{
        sessionCheckFlag = 0;
    }
    free(sql);
}

void updateSession(char* sessionID, char* tokenStr, sqlite3* db)
{
    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    time_t t;
    t=time(&t);

    strcat(sql, "UPDATE session SET token=\'");
    strcat(sql, tokenStr);
    strcat(sql, "\', time=\'");
    strcat(sql, (char*)ctime(&t));
    strcat(sql, "\' WHERE ID=");
    strcat(sql, sessionID);
    strcat(sql, ";");

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        printf("updateSession error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

void makeSession(char* user_accessID, char* tokenStr, sqlite3* db)
{
    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(1000);
    memset(sql, 0, 1000);

    time_t t;
    t=time(&t);
    
    strcat(sql, "INSERT INTO session (token,user_access_id,time) VALUES ( \'");
    strcat(sql, tokenStr);
    strcat(sql, "\' , \'");
    strcat(sql, user_accessID);
    strcat(sql, "\' , \'");
    strcat(sql, (char*)ctime(&t));
    strcat(sql, "\');");
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
    printf("session SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    }

    free(sql);
}
