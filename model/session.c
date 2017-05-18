#include "session.h"

char sessionCheckFlag = 0;

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