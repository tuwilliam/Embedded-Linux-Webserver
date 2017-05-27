#include "../common/common.h"
#include "../common/APP.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColusername){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColusername[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void tableDrop(sqlite3* db, char* tabelusername){

  int rc;
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

void tableCreate_user_access(sqlite3* db){
  
  int rc;
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

void tableCreate_session(sqlite3* db){

  int rc;
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

void tableCreate_switch(sqlite3* db){

  int rc;
  char* sql = malloc(500);
  memset(sql, 0, 500);
  char* zErrMsg = 0;
  strcat(sql, "CREATE TABLE switch("  \
         "ID INTEGER PRIMARY KEY     AUTOINCREMENT," \
         "MAC_DA         INT    NOT NULL," \
         "MAC_SA         INT    NOT NULL," \
         "IPv4           INT    NOT NULL," \
         "IP_PROTOCOL    INT    NOT NULL," \
         "IP_SA          INT    NOT NULL," \
         "IP_DA          INT    NOT NULL," \
         "Port_S         INT    NOT NULL," \
         "Port_D         INT    NOT NULL," \
         "UDP            INT    NOT NULL," \
         "TCP            INT    NOT NULL," \
         "ARP            INT    NOT NULL," \
         "ICMP           INT    NOT NULL," \
         "SIP            INT    NOT NULL," \
         "NAT            INT    NOT NULL);");
         
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "session SQL creat error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table switch created successfully\n");
   }
   free(sql);
   sql = NULL;
}

void tableCreate_mac(sqlite3* db){

  int rc;
  char* sql = malloc(500);
  memset(sql, 0, 500);
  char* zErrMsg = 0;
  strcat(sql, "CREATE TABLE mac("  \
         "ID INTEGER PRIMARY KEY     AUTOINCREMENT," \
         "ETH            INT    NOT NULL," \
         "ADDR           INT    NOT NULL," \
         "MAC_5          INT    NOT NULL," \
         "MAC_4          INT    NOT NULL," \
         "MAC_3          INT    NOT NULL," \
         "MAC_2          INT    NOT NULL," \
         "MAC_1          INT    NOT NULL," \
         "MAC_0          INT    NOT NULL);");
         
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "session SQL creat error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table mac created successfully\n");
   }
   free(sql);
   sql = NULL;
}

void rowInsert_user_access(sqlite3* db){
  
  int rc;
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

void rowInsert_session(sqlite3* db){
  
  int rc;
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

void rowInsert_switch(sqlite3* db){
  
  int rc;
  char* sql = malloc(1000);
  memset(sql, 0, 1000);
  strcat(sql,"INSERT INTO switch (ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, " \
      "Port_D, UDP, TCP, ARP, ICMP, SIP, NAT) "  \
      "VALUES (1, 0, 1, 0,    0, 0, 0, 0,     0, 0, 0, 0,    0, 0, 0); " \
      "INSERT INTO switch (ID, MAC_DA, MAC_SA, IPv4, IP_PROTOCOL, IP_SA, IP_DA, Port_S, " \
      "Port_D, UDP, TCP, ARP, ICMP, SIP, NAT) "  \
      "VALUES (2, 0, 0, 0,    0, 0, 0, 0,     0, 0, 0, 0,    0, 0, 0); ");
  char* zErrMsg = 0;
  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    fprintf(stderr, "switch SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }else{
    fprintf(stdout, "switch Records Insert successfully\n");
  }
  free(sql);
  sql = NULL;
}

char ADDR_IN[16][3] = {"32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47"};
char ADDR_OUT[16][4] = {"160", "161", "162", "163", "164", "165", "166", "167", "168", "169", "170", "171", "172", "173", "174", "175"};

void rowInsert_mac(sqlite3* db){
  
  int rc, i;
  char* sql = malloc(1000);

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO mac (ETH, ADDR, MAC_5, MAC_4, MAC_3, MAC_2, MAC_1, MAC_0) VALUES (0, ");
    strcat(sql, ADDR_IN[i]);
    strcat(sql, ", 0, 0, 0, 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "switch SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "switch Records Insert successfully\n");
    }
  }

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO mac (ETH, ADDR, MAC_5, MAC_4, MAC_3, MAC_2, MAC_1, MAC_0) VALUES (1, ");
    strcat(sql, ADDR_OUT[i]);
    strcat(sql, ", 0, 0, 0, 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "switch SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "switch Records Insert successfully\n");
    }
  }
  
  free(sql);
  sql = NULL;
}

void tableInit(sqlite3* db){

    tableDrop(db, "user_access");
    tableDrop(db, "session");
    tableDrop(db, "switch");
    tableDrop(db, "mac");

    tableCreate_user_access(db);
    rowInsert_user_access(db);

    tableCreate_session(db);
    rowInsert_session(db);

    tableCreate_switch(db);
    rowInsert_switch(db);

    tableCreate_mac(db);
    rowInsert_mac(db);
}

int main(int argc, char* argv[]){
   sqlite3* db;
   char* zErrMsg = 0;
   int  rc;

   /* Open database */
   rc = sqlite3_open("netgap.db", &db);

   if(rc){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      exit(1);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }

   tableInit(db);

   sqlite3_close(db);
   return 0;
}