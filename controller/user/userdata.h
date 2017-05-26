#include "../../common/common.h"

typedef struct userData
{
	char* username;
	char* pwd;
}userData;

/*golbal sqlite3 variable*/
sqlite3* db;

/* find username value & password value in JSON string*/
void  getJSON(char* input, char* username, char* pwd);
/* make JSON string with value status, token, user_access_id*/
char* makeJSON(char* status, char* tokenStr , char* user_access_id);
/* check if user_access_id is in table session*/
void checkUserSession(char* user_access_id, sqlite3* db);
/*write a 8digits number as a string to tokenstr*/
void makeToken(char* tokenStr);
/*update user session in the session table*/
void makeSession(char* user_accessID, char* tokenStr, sqlite3* db);
/*update user session in the session table*/
void updateSession(char* sessionID, char* tokenStr, sqlite3* db);


