#include "../../common/common.h"

static int callback(void *data, int argc, char **argv, char **azColName){

    if(strcmp(argv[2], data) == 0){
        //makeSession(argv[0]);
        printf("%s\n", makeJSON());
        exit(1);
    }
    else{
        printf("Incorrect Password!\n");
        exit(1);
    }
    
    return 0;
}

int main(void)
{
	char* input = malloc(100);
    char* sql = malloc(100);

    userData newuser;
    newuser.username = malloc(100);
    newuser.pwd = malloc(100);

    memset(input, 0, 100);
    memset(sql, 0, 100);
    memset(newuser.username, 0, 100);
    memset(newuser.pwd, 0, 100);

    char* method;
    char* uri;
    // char username[100] = {0};
    // char pwd[100] = {0};
    sqlite3* db;
    char* zErrMsg = 0;
    int  rc;
    

	
    printf("Content-type:text/html\n\n");
    
    method = getenv("REQUEST_METHOD");
    uri = getenv("REQUEST_URI");
    input = getCgiData(stdin, method);


    char* subRouter = getCharPos(uri, "/", 4);

    rc = sqlite3_open("../model/netgap.db", &db);
    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(input);
        free(sql);
        free(newuser.username);
        free(newuser.pwd);
        input = NULL;
        sql = NULL;
        newuser.username = NULL;
        newuser.pwd = NULL;
        exit(1);
    }
    
    if(strcmp(subRouter, "login") == 0){

        getjSON(input, newuser.username, newuser.pwd);
 
        strcat(sql, "SELECT * FROM user_access WHERE username=\"");
        strcat(sql, newuser.username);
        strcat(sql, "\";");

        rc = sqlite3_exec(db, sql, callback, newuser.pwd, &zErrMsg);

        if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
        }else{
          fprintf(stdout, "Incorrect Username!\n");
        }

    } 

    sqlite3_close(db);
    free(input);
    free(sql);
    free(newuser.username);
    free(newuser.pwd);
    input = NULL;
    sql = NULL;
    newuser.username = NULL;
    newuser.pwd = NULL;
	return 0;
}
