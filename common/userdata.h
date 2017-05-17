#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <time.h>
#include "sqlite3.h"

typedef struct userData
{
	char* username;
	char* pwd;
}userData;

void  getjSON(char* input, char* username, char* pwd);
char* makeJSON();

void getSessionID(char* user_access_id, sqlite3* db, int rc);
void makeToken();
void makeSession();
void updateSession(char* sessionID, char* tokenStr, sqlite3* db, int rc);

sqlite3* db;
int  rc;
