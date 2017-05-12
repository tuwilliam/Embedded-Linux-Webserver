#include "../../common/common.h"

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

static int callback(void *data, int argc, char **argv, char **azColName){

    if(strcmp(argv[2], data) == 0){
        printf("Success!\n");
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
	char* input;
	char* method;
    char* uri;
    char username[100] = {0};
    char pwd[100] = {0};



    char* temp;
    int i;


    sqlite3* db;
    char* zErrMsg = 0;
    int  rc;
    char sql[100] = {0};

	
    printf("Content-type:text/html\n\n");
    input = malloc(100);
    
    method = getenv("REQUEST_METHOD");
    input = getCgiData(stdin, method);

    uri = getenv("REQUEST_URI");

    char* subRouter = getCharPos(uri, "/", 4);

    rc = sqlite3_open("../model/netgap.db", &db);
    if(rc){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      exit(1);
    }
    
    if(strcmp(subRouter, "login") == 0){

        getjSON(input, username, pwd);
 
        strcat(sql, "SELECT * FROM user_access WHERE username=\"");
        strcat(sql, username);
        strcat(sql, "\";");

        rc = sqlite3_exec(db, sql, callback, pwd, &zErrMsg);

        if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
        }else{
          fprintf(stdout, "Incorrect Username\n");
        }

    } 

    sqlite3_close(db);
	return 0;
}
