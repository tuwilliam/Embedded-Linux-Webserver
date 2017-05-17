#include "session.h"

char sessionCheck = 0;

static int sessionCheckCallback(void *successFuncInCB, int argc, char **argv, char **azColName)
{
    successFuncInCB -> func(successFuncInCB.argc, successFuncInCB.argv);
    // if(strcmp(argv[2], pwd) == 0){
    //     getSessionID(argv[0], db, rc);
    // }
    // else{
    //     printf("%s\n", makeJSON("Incorrect Password!", "", ""));
    //     exit(1);
    // }
    // return 0;
}

int sessionCheck(char* user_accessID, char* token, SessionCheckFun* successFunc, SessionCheckFun* failFunc, sqlite3* db, int rc)
{
	char* sql = malloc(100);
    memset(sql, 0, 100);
    char* zErrMsg = 0;
    char* sessionID = NULL;

    char tokenStr[8];
    makeToken(tokenStr);

    strcat(sql, "SELECT * FROM session WHERE user_access_id=");
    strcat(sql, user_access_id);
    strcat(sql, "AND token=");
    strcat(sql, token);
    strcat(sql, ";");

    rc = sqlite3_exec(db, sql, sessionCheckCallback, successFunc, &zErrMsg);
    
    if( rc != SQLITE_OK ){
      printf("session Table error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      sessionID = NULL;
    }

    if(!sessionCheck){
        makeSession(user_access_id, tokenStr, db, rc);
        printf("%s\n", makeJSON("success", tokenStr, user_access_id));
    }
    else{
        sessionCheck = 0;
    }
    free(sql);
}