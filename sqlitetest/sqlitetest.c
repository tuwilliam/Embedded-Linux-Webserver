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

void tableCreate(sqlite3* db,  int  rc, char* tabelusername){
    // char* sql = "CREATE TABLE ";
    // int n = snprintf(sql, 500, "%s%s%s", sql, tabelusername, "("  \
    //      "ID INT PRIMARY KEY     NOT NULL," \
    //      "username           TEXT    NOT NULL," \
    //      "pwd            TEXT     NOT NULL," \
    //      "ACCESS         INT);");
  char* zErrMsg = 0;
  char* sql = "CREATE TABLE user_access("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "username           TEXT    NOT NULL," \
         "pwd            TEXT     NOT NULL," \
         "ACCESS         INT);";
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
}

void tableDrop(sqlite3* db,  int  rc, char* tabelusername){
  // char* sql = "DROP TABLE ";

  //int n = snprintf(sql, 100, "%s%s%s", sql, tabelusername, ";");
  // char* sql = "DROP TABLE user_access; ";
  // char* sql = "DROP TABLE ";
  char sql[100] = {0};
  //memset(sql, 100 ,0);
  strcat(sql, "DROP TABLE ");
  strcat(sql, tabelusername);
  strcat(sql, ";");
  char* zErrMsg = 0;

  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table user_access Drop successfully\n");
   }
}

void rowInsert(sqlite3* db,  int  rc){

    char* sql = "INSERT INTO user_access (ID,username,pwd,ACCESS) "  \
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
         "VALUES (6, 'userLevel5', 'lab555', 5);";
    char* zErrMsg = 0;

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Records Insert successfully\n");
   }
}

void tableInit(sqlite3* db,  int  rc){
    tableDrop(db, rc, "user_access");
    tableCreate(db, rc, "user_access");
    rowInsert(db, rc);
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
   // /* Open database */
   // if(argc){

   //    if(argv[0] == "init" || argv[0] == "INIT"){
   //        tableInit();
   //        return 0;         
   //    }  

   //    if(argv[0] == "drop" || argv[0] == "DROP"){
   //        tableDrop(db, rc, argv[1]);
   //        return 0;         
   //    }


   //    if(argv[0] == "insert" || argv[0] == "INSERT"){
   //        tableInsert(db, rc);
   //        return 0;
   //    }
   // }
   sqlite3_close(db);
   return 0;
}