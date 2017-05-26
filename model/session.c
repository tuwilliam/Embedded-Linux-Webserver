#include "session.h"

/*Flag indicates if table session have the user_access_id & token*/
char sessionCheckFlag = 0;

/*
*If find user_access_id & token in table session, enter callback, update session ID
*@parameter pwd is the token string need to be updated
*@parameter argc is the variable number
*@parameter argv is the variable array(session_id, token, user_access_id, time)
*@parameter azColusername is the variable name array("ID", "token", "user_access_id","time")
*@author SUN ZHOGNJIAN
*/
static int sessionCheckCallback(SessionCheckFun* successFuncInCB, int argc, char **argv, char **azColName){
    sessionCheckFlag = 1;
    successFuncInCB -> func(successFuncInCB -> argc, successFuncInCB -> argv);
    return 0;
}

int sessionCheck(char* user_accessID, char* token, SessionCheckFun* successFunc, SessionCheckFun* failFunc, sqlite3* db, int rc){

	char* sql = malloc(100);
    memset(sql, 0, 100);
    char* zErrMsg = 0;

    strcat(sql, "SELECT * FROM session WHERE user_access_id=");
    strcat(sql, user_accessID);
    strcat(sql, " AND token=");
    strcat(sql, token);
    strcat(sql, ";");

    printf("%s   ", sql);

    rc = sqlite3_exec(db, sql, sessionCheckCallback, successFunc, &zErrMsg);
    
    printf("%d   ", rc);

    if( rc != SQLITE_OK ){
      printf("session Table error: %s   ", zErrMsg);
      sqlite3_free(zErrMsg);
    }

    if(!sessionCheckFlag){
        failFunc -> func(failFunc -> argc, failFunc -> argv);
    }
    else{
        sessionCheckFlag = 0;
    }
    free(sql);
}