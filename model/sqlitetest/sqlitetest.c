#include "../../common/common.h"
#include "../../common/APP.h"

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
   fprintf(stderr, "switch SQL creat error: %s\n", zErrMsg);
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
   fprintf(stderr, "mac SQL creat error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table mac created successfully\n");
   }
   free(sql);
   sql = NULL;
}

void tableCreate_nat(sqlite3* db){

  int rc;
  char* sql = malloc(500);
  memset(sql, 0, 500);
  char* zErrMsg = 0;
  strcat(sql, "CREATE TABLE nat("  \
         "ID INTEGER PRIMARY KEY     AUTOINCREMENT," \
         "ETH            INT    NOT NULL," \
         "ADDR           INT    NOT NULL," \
         "SIP_3          INT    NOT NULL," \
         "SIP_2          INT    NOT NULL," \
         "SIP_1          INT    NOT NULL," \
         "SIP_0          INT    NOT NULL," \
         "DIP_3          INT    NOT NULL," \
         "DIP_2          INT    NOT NULL," \
         "DIP_1          INT    NOT NULL," \
         "DIP_0          INT    NOT NULL);"); 
         
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "nat SQL creat error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table nat created successfully\n");
   }
   free(sql);
   sql = NULL;
}

void tableCreate_port(sqlite3* db){

  int rc;
  char* sql = malloc(500);
  memset(sql, 0, 500);
  char* zErrMsg = 0;
  strcat(sql, "CREATE TABLE port("  \
         "ID INTEGER PRIMARY KEY     AUTOINCREMENT," \
         "ETH            INT    NOT NULL," \
         "ADDR           INT    NOT NULL," \
         "PORT_H         INT    NOT NULL," \
         "PORT_L         INT    NOT NULL," \
         "PROTOCOL       INT    NOT NULL);"); 
         
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "port SQL creat error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table port created successfully\n");
   }
   free(sql);
   sql = NULL;
}

void tableCreate_ip(sqlite3* db){

  int rc;
  char* sql = malloc(500);
  memset(sql, 0, 500);
  char* zErrMsg = 0;
  strcat(sql, "CREATE TABLE ip("  \
         "ID INTEGER PRIMARY KEY     AUTOINCREMENT," \
         "ETH            INT    NOT NULL," \
         "ADDR           INT    NOT NULL," \
         "IP_3           INT    NOT NULL," \
         "IP_2           INT    NOT NULL," \
         "IP_1           INT    NOT NULL," \
         "IP_0           INT    NOT NULL);"); 
         
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "port SQL creat error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table ip created successfully\n");
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

char MAC_ADDR_IN[16][3] = {"32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47"};
char MAC_ADDR_OUT[16][4] = {"160", "161", "162", "163", "164", "165", "166", "167", "168", "169", "170", "171", "172", "173", "174", "175"};

void rowInsert_mac(sqlite3* db){
  
  int rc, i;
  char* sql = malloc(1000);

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO mac (ETH, ADDR, MAC_5, MAC_4, MAC_3, MAC_2, MAC_1, MAC_0) VALUES (0, ");
    strcat(sql, MAC_ADDR_IN[i]);
    strcat(sql, ", 0, 0, 0, 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "mac SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "mac Records Insert successfully\n");
    }
  }

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO mac (ETH, ADDR, MAC_5, MAC_4, MAC_3, MAC_2, MAC_1, MAC_0) VALUES (1, ");
    strcat(sql, MAC_ADDR_OUT[i]);
    strcat(sql, ", 0, 0, 0, 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "mac SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "mac Records Insert successfully\n");
    }
  }
  
  free(sql);
  sql = NULL;
}

char NAT_ADDR_IN[16][4] = {"112", "113", "114", "115", "116", "117", "118", "119", "120", "121", "122", "123", "124", "125", "126", "127"};
char NAT_ADDR_OUT[16][4] = {"240", "241", "242", "243", "244", "245", "246", "247", "248", "249", "250", "251", "252", "253", "254", "255"};

void rowInsert_nat(sqlite3* db){
  
  int rc, i;
  char* sql = malloc(1000);

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO nat (ETH, ADDR, SIP_3, SIP_2, SIP_1, SIP_0, DIP_3, DIP_2, DIP_1, DIP_0) VALUES (0, ");
    strcat(sql, NAT_ADDR_IN[i]);
    strcat(sql, ", 0, 0, 0, 0, 0, 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "nat SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "nat Insert successfully\n");
    }
  }

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO nat (ETH, ADDR, SIP_3, SIP_2, SIP_1, SIP_0, DIP_3, DIP_2, DIP_1, DIP_0) VALUES (1, ");
    strcat(sql, NAT_ADDR_OUT[i]);
    strcat(sql, ", 0, 0, 0, 0, 0, 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "nat SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "nat Insert successfully\n");
    }
  }
  
  free(sql);
  sql = NULL;
}

char PORT_ADDR_IN_S[16][3] = {"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "90", "91", "92", "93", "94", "95"};
char PORT_ADDR_IN_D[16][4] = {"96", "97", "98", "99", "100", "101", "102", "103", "104", "105", "106", "107", "108", "109", "110", "111"};
char PORT_ADDR_OUT_S[16][4] = {"208", "209", "210", "211", "212", "213", "214", "215", "216", "217", "218", "219", "220", "221", "222", "223"};
char PORT_ADDR_OUT_D[16][4] = {"224", "225", "226", "227", "228", "229", "230", "231", "232", "233", "234", "235", "236", "237", "238", "239"};

void rowInsert_port(sqlite3* db){
  
  int rc, i;
  char* sql = malloc(1000);

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO port (ETH, ADDR, PORT_H, PORT_L, PROTOCOL) VALUES (0, ");
    strcat(sql, PORT_ADDR_IN_S[i]);
    strcat(sql, ", 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "port SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "port Insert successfully\n");
    }
  }

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO port (ETH, ADDR, PORT_H, PORT_L, PROTOCOL) VALUES (0, ");
    strcat(sql, PORT_ADDR_IN_D[i]);
    strcat(sql, ", 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "port SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "port Insert successfully\n");
    }
  }

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO port (ETH, ADDR, PORT_H, PORT_L, PROTOCOL) VALUES (1, ");
    strcat(sql, PORT_ADDR_OUT_S[i]);
    strcat(sql, ", 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "port SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "port Insert successfully\n");
    }
  }

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO port (ETH, ADDR, PORT_H, PORT_L, PROTOCOL) VALUES (1, ");
    strcat(sql, PORT_ADDR_OUT_D[i]);
    strcat(sql, ", 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "port SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "port Insert successfully\n");
    }
  } 
  free(sql);
  sql = NULL;
}

char IP_ADDR_IN_S[16][3] = {"48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61", "62", "63"};
char IP_ADDR_OUT_D[16][4] = {"192", "193", "194", "195", "196", "197", "198", "199", "200", "201", "202", "203", "204", "205", "206", "207"};

void rowInsert_ip(sqlite3* db){
  
  int rc, i;
  char* sql = malloc(1000);

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO ip (ETH, ADDR, IP_3, IP_2, IP_1, IP_0) VALUES (0, ");
    strcat(sql, IP_ADDR_IN_S[i]);
    strcat(sql, ", 0, 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "ip SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "ip Insert successfully\n");
    }
  }

  for(i = 0; i < 16; i++){
    memset(sql, 0, 1000);
    strcat(sql, "INSERT INTO ip (ETH, ADDR, IP_3, IP_2, IP_1, IP_0) VALUES (1, ");
    strcat(sql, IP_ADDR_OUT_D[i]);
    strcat(sql, ", 0, 0, 0, 0);");
    char* zErrMsg = 0;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "ip SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
      fprintf(stdout, "ip Insert successfully\n");
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
    tableDrop(db, "nat");
    tableDrop(db, "port");
    tableDrop(db, "port");

    tableCreate_user_access(db);
    rowInsert_user_access(db);

    tableCreate_session(db);
    rowInsert_session(db);

    tableCreate_switch(db);
    rowInsert_switch(db);

    tableCreate_mac(db);
    rowInsert_mac(db);

    tableCreate_nat(db);
    rowInsert_nat(db);

    tableCreate_port(db);
    rowInsert_port(db);

    tableCreate_ip(db);
    rowInsert_ip(db);
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