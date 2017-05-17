#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <string.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColusername){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColusername[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void tableCreate_user_access(sqlite3* db,  int  rc){

  char* sql = malloc(200);
  memset(sql, 0, 200);
  char* zErrMsg = 0;
  strcat(sql, "CREATE TABLE user_access("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "username           TEXT    NOT NULL," \
         "pwd            TEXT     NOT NULL," \
         "ACCESS         INT);");
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "user_access SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table user_access created successfully\n");
   }
   free(sql);
   sql = NULL;
}

void tableCreate_session(sqlite3* db,  int  rc){

  char* sql = malloc(500);
  memset(sql, 0, 500);
  char* zErrMsg = 0;
  strcat(sql, "CREATE TABLE session("  \
         "ID INTEGER PRIMARY KEY     AUTOINCREMENT," \
         "token           TEXT    NOT NULL," \
         "user_access_id INT NOT NULL," \
         "time            TEXT NOT NULL,"\
         "FOREIGN KEY ( user_access_id ) REFERENCES user_access_( user_access_id ));");
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "session SQL creat error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table session created successfully\n");
   }
   free(sql);
   sql = NULL;
}

void tableDrop(sqlite3* db,  int  rc, char* tabelusername){

  char* sql = malloc(100);
  memset(sql, 0, 100);
  strcat(sql, "DROP TABLE ");
  strcat(sql, tabelusername);
  strcat(sql, ";");
  char* zErrMsg = 0;

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
   fprintf(stderr, "%s SQL drop error: %s\n", tabelusername, zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table user_access Drop successfully\n");
   }
   free(sql);
   sql = NULL;
}

void rowInsert_user_access(sqlite3* db,  int  rc){
  char* sql = malloc(1000);
  memset(sql, 0, 1000);
  strcat(sql,"INSERT INTO user_access (ID,username,pwd,ACCESS) "  \
       "VALUES (1, 'root', 'lab555', 0); " \
       "INSERT INTO user_access (ID,username,pwd,ACCESS) "  \
       "VALUES (2, 'userLevel1', 'lab555', 1); " \
       "INSERT INTO user_access (ID,username,pwd,ACCESS) "  \
       "VALUES (3, 'userLevel2', 'lab555', 2); " \
       "INSERT INTO user_access (ID,username,pwd,ACCESS) "  \
       "VALUES (4, 'userLevel3', 'lab555', 3);" \
       "INSERT INTO user_access (ID,username,pwd,ACCESS) "  \
       "VALUES (5, 'userLevel4', 'lab555', 4);" \
       "INSERT INTO user_access (ID,username,pwd,ACCESS) "  \
       "VALUES (6, 'userLevel5', 'lab555', 5);");
  char* zErrMsg = 0;
  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    fprintf(stderr, "user_access SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }else{
    fprintf(stdout, "user_access Records Insert successfully\n");
  }
  free(sql);
  sql = NULL;
}

void rowInsert_session(sqlite3* db,  int  rc){
  char* sql = malloc(1000);
  memset(sql, 0, 1000);
  strcat(sql,"INSERT INTO session (ID,token,user_access_id,time) "  \
       "VALUES (1, '12345678', '2', '20:00'); " );
  char* zErrMsg = 0;
  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    fprintf(stderr, "session SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }else{
    fprintf(stdout, "session Records Insert successfully\n");
  }
  free(sql);
  sql = NULL;
}

void tableUpdate_session(sqlite3* db,  int  rc){

  char* sql = malloc(100);
    memset(sql, 0, 100);

    char* zErrMsg = 0;
    strcat(sql, "UPDATE session SET token='98123543' WHERE ID=1;");
    // strcat(sql, "UPDATE session SET token=\'");
    // strcat(sql, tokenStr);
    // strcat(sql, "\' WHERE ID=");
    // strcat(sql, sessionID);
    // strcat(sql, ";\n");
    printf("%s\n", sql);
    //rc = sqlite3_open("../model/netgap.db", &db);
    rc = sqlite3_open("../model/netgap.db", &db);
    printf("rc open = %d\n",rc);
    if(rc){
        printf("updateSession Can't open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    sqlite3_close(db);
    printf("rc = %d\n",rc);
    if( rc != SQLITE_OK ){
        printf("updateSession error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else
      fprintf(stdout, "session update successfully\n");

    free(sql);
}

void tableInit(sqlite3* db,  int  rc){
    tableDrop(db, rc, "user_access");
    tableDrop(db, rc, "session");
    tableCreate_user_access(db, rc);
    rowInsert_user_access(db, rc);
    tableCreate_session(db, rc);
    rowInsert_session(db, rc);
    tableUpdate_session(db, rc);
}

int main(int argc, char* argv[])
{
   sqlite3* db;
   char* zErrMsg = 0;
   int  rc;
   char* sql;

   /* Open database */
   rc = sqlite3_open("netgap.db", &db);

   if(rc){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      exit(1);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }

   tableInit(db, rc);

   sqlite3_close(db);
   return 0;
}