#include "userdata.h"

/*
*Get jSON value from jSON string
*@parameter input, the input JSON string
*@parameter username, put username object value into this parameter
*@parameter pwd, put username object value into this parameter
*@author SUN ZHOGNJIAN
*/
void getJSON(char* input, char* username, char* pwd){
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

/*
*Convert 3 strings to a jSON string
*@parameter status, tokenStr, user_access_id is the strings need to make to JSON
*@author SUN ZHOGNJIAN
*/
char* makeJSON(char* status, char* tokenStr , char* user_access_id){
    cJSON* pJSON = NULL;

    pJSON = cJSON_CreateObject();
    if(pJSON == NULL){
        //Error happend here
        return NULL;
    }
    cJSON_AddStringToObject(pJSON, "status", status);
    cJSON_AddStringToObject(pJSON, "token", tokenStr);
    cJSON_AddStringToObject(pJSON, "id", user_access_id);

    char* p = cJSON_Print(pJSON);
    if(p == NULL){
        //Convert json list to string faild, exit
        //Because sub json pSubJson han been add to pJSON, so just delete pJSON, if you also delete pSubJson, it will coredump, and error is : double free
        cJSON_Delete(pJSON);
        return NULL;
    }
    
    cJSON_Delete(pJSON);

    return p;
}

/*
*Write a 8digits number as a string to tokenstr
*@parameter tokenStr is the token string need to be written
*@author SUN ZHOGNJIAN
*/
void makeToken(char* tokenStr){
    srand((unsigned) time(NULL));
    sprintf(tokenStr, "%d", rand() % 100000000);//1_0000_0000
}

/*Flag indicates if table session have the user_access_id*/
char checkUserSessionFlag = 0;

/*
*If find user_access_id in table session, enter callback, update session ID
*@parameter pwd is the token string need to be updated
*@parameter argc is the variable number
*@parameter argv is the variable array(session_id, token, user_access_id, time)
*@parameter azColusername is the variable name array("ID", "token", "user_access_id","time")
*@author SUN ZHOGNJIAN
*/
static int checkUserSessionCallback(void *tokenStr, int argc, char **argv, char **azColusername){   
    //if user_access_id is in table session, user have logged, flag = 1
    checkUserSessionFlag = 1;
    //user have logged, the session need to be updated
    updateSession(argv[0], (char*)tokenStr, db);
    //return log status, token string, user_id to web front-end
    printf("%s\n", makeJSON("success", (char*)tokenStr, argv[2]));
    return 0;
}

/*
*check if user_access_id is in table session ? (logged)update session : (never logged)make session
*@parameter user_access_id is user_access_id
*@parameter db is the netgap database
*@author SUN ZHOGNJIAN
*/
void checkUserSession(char* user_access_id, sqlite3* db){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    //make a random 8byte number as token string
    char tokenStr[8];
    makeToken(tokenStr);
    //check if the user_access_id is in table session
    strcat(sql, "SELECT * FROM session WHERE user_access_id=");
    strcat(sql, user_access_id);
    strcat(sql, ";");

    rc = sqlite3_exec(db, sql, checkUserSessionCallback, tokenStr, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
      printf("session Table error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    //the user_id is not in the session table, this is the first time user login
    if(!checkUserSessionFlag){
        //make user session in the session table
        makeSession(user_access_id, tokenStr, db);
        //return log status, token string, user_id to web front-end
        printf("%s\n", makeJSON("success", tokenStr, user_access_id));
    }
    else{
        //reset flag
        checkUserSessionFlag = 0;
    }
    free(sql);
}

/*
*update user session in the session table
*@parameter sessionID is the to-be-updated session's ID
*@parameter tokenStr is the new token string need to be updated
*@parameter db is the netgap database
*@author SUN ZHOGNJIAN
*/
void updateSession(char* sessionID, char* tokenStr, sqlite3* db){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(100);
    memset(sql, 0, 100);

    //get log time, and update it in session table
    time_t t;
    t=time(&t);

    //update session according to session id
    strcat(sql, "UPDATE session SET token=\'");
    strcat(sql, tokenStr);
    strcat(sql, "\', time=\'");
    strcat(sql, (char*)ctime(&t));
    strcat(sql, "\' WHERE ID=");
    strcat(sql, sessionID);
    strcat(sql, ";");

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
        printf("updateSession error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    free(sql);
}

/*
*update user session in the session table
*@parameter sessionID is the to-be-updated session's ID
*@parameter tokenStr is the new token string need to be updated
*@parameter db is the netgap database
*@author SUN ZHOGNJIAN
*/
void makeSession(char* user_accessID, char* tokenStr, sqlite3* db){

    int rc;
    char* zErrMsg = 0;

    char* sql = (char*)malloc(1000);
    memset(sql, 0, 1000);

    //get log time, and update it in session table
    time_t t;
    t=time(&t);
    
    //make new session, the session ID can be autoincresed
    strcat(sql, "INSERT INTO session (token,user_access_id,time) VALUES ( \'");
    strcat(sql, tokenStr);
    strcat(sql, "\' , \'");
    strcat(sql, user_accessID);
    strcat(sql, "\' , \'");
    strcat(sql, (char*)ctime(&t));
    strcat(sql, "\');");

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    //Access database error
    if( rc != SQLITE_OK ){
    printf("session SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    }
    free(sql);
}
