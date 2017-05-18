#include "../../common/common.h"

typedef struct userData
{
	char* username;
	char* pwd;
}userData;

sqlite3* db;
int  rc;

void  getjSON(char* input, char* username, char* pwd);
char* makeJSON();

void checkUserSession(char* user_access_id, sqlite3* db);
void makeToken();
void makeSession(char* user_accessID, char* tokenStr, sqlite3* db);
void updateSession(char* sessionID, char* tokenStr, sqlite3* db);


