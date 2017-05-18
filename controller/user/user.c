#include "../../common/common.h"
#include "../../common/APP.h"

/*Flag indicates if table user_access have the username*/
char nameCheckFlag = 0;

/*
*If find username in table user_access, enter callback, check password
*@parameter pwd is the user password string from web input
*@parameter argc is the variable number
*@parameter argv is the variable array(user_id, user_name, user_pwd,user_access)
*@parameter azColusername is the variable name array("ID", "username", "pwd","ACCESS")
*@author SUN ZHOGNJIAN
*/
static int usernameCallback(void *pwd, int argc, char **argv, char **azColName)
{       
    /*find username, Flag = 1*/
    nameCheckFlag = 1;
    /*password correct*/
    if(strcmp(argv[2], pwd) == 0){
        /*getSessionID, check if it has logged*/
        checkUserSession(argv[0], db);
    }
    /*password incorrect*/
    else{
        printf("%s\n", makeJSON("Incorrect Password!", "", ""));
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

    char* zErrMsg = 0;

    printf("Content-type:text/html\n\n");
    
    //get request method
    method = getenv("REQUEST_METHOD");    
    //get request uri  
    uri = getenv("REQUEST_URI");
    //get request content which should be a jSON string
    input = getCgiData(stdin, method);      

    //get uri sub string after the fourth "/"
    char* subRouter = getCharPos(uri, "/", 4);

    rc = sqlite3_open("../model/netgap.db", &db);
    //error when open DB
    if(rc){
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
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
    //if it is login request
    if(strcmp(subRouter, "login") == 0){
        //get username and password from input jSON string
        getjSON(input, newuser.username, newuser.pwd);

        //check if the username exist
        strcat(sql, "SELECT * FROM user_access WHERE username=\"");
        strcat(sql, newuser.username);
        strcat(sql, "\";");

        rc = sqlite3_exec(db, sql, usernameCallback, newuser.pwd, &zErrMsg);
        //when sql format is incorrect
        if( rc != SQLITE_OK ){
          printf("SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
        }

        //if nameCheckFlag is 0, the callback function is not executed, the username is incorrect
        if(!nameCheckFlag){
            printf("%s\n", makeJSON("Incorrect Username!", "", ""));
        }else{
            nameCheckFlag = 0;
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
