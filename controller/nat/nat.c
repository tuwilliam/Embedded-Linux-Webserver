#include "../../common/common.h"
#include "../../common/APP.h"

/*Golbal sqlite3 variable*/
sqlite3* db;

int main(int argc, char** argv){

    char* method;
    char* uri;

    //Print debug infomation to web
    printf("Content-type:text/html\n\n");

    //Get request method
    method = getenv("REQUEST_METHOD");  

    //Get request uri  
    uri = getenv("REQUEST_URI");
    char* subRouter = getCharPos(uri, "/", 4);


    char* zErrMsg = 0;
    int  rc;

    /* Open database */
    rc = sqlite3_open("../model/netgap.db", &db);
    //Error when open DB
    if(rc){
      printf("Can't open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      exit(1);
    }

    //If the web is getting switch request
    if(strcmp(subRouter, "switch") == 0 && strcmp(method, "GET") == 0){
        getNatSwitch();
    }
    //If the web is changing nat switch switch to open or close
    if(strcmp(subRouter, "switch/true") == 0 || strcmp(subRouter, "switch/false") == 0 && strcmp(method, "POST") == 0){
        setNatSwitch(getCharPos(uri, "/", 5));
    }

    //If the web is getting natlist in database
    if(strcmp(subRouter, "getNatList") == 0 && strcmp(method, "GET") == 0){
        getNat();
    }

    

    //If the web is setting natlist in database
    if(strcmp(subRouter, "setNatRow") == 0 && strcmp(method, "POST") == 0){
        printf("   setNatRow  ");
        char id[10], eth[10], addr[10], SIP_3[10], SIP_2[10], SIP_1[10], SIP_0[10], DIP_3[10], DIP_2[10], DIP_1[10], DIP_0[10];
        char* input = malloc(100);
        memset(input, 0 ,100);
        memset(id, 0 ,10);
        memset(eth, 0 ,10);
        memset(addr, 0 ,10);
        memset(SIP_3, 0 ,10);
        memset(SIP_2, 0 ,10);
        memset(SIP_1, 0 ,10);
        memset(SIP_0, 0 ,10);
        memset(DIP_3, 0 ,10);
        memset(DIP_2, 0 ,10);
        memset(DIP_1, 0 ,10);
        memset(DIP_0, 0 ,10);

        input = getCgiData(stdin, method);  
   
        getNatJSON(input, id, eth, addr, SIP_3, SIP_2, SIP_1, SIP_0, DIP_3, DIP_2, DIP_1, DIP_0);

        if(strcmp(eth, "0") == 0){
            setNatRow(id, eth, addr, SIP_3, SIP_2, SIP_1, SIP_0, DIP_3, DIP_2, DIP_1, DIP_0);
            usleep(30000);
            int temp = atoi(addr) + 136;
            sprintf(addr, "%d", temp);
            setNatRow(id, eth, addr, DIP_3, DIP_2, DIP_1, DIP_0, SIP_3, SIP_2, SIP_1, SIP_0);
        }else if(strcmp(eth, "1") == 0){
            setNatRow(id, eth, addr, SIP_3, SIP_2, SIP_1, SIP_0, DIP_3, DIP_2, DIP_1, DIP_0);
            usleep(30000);
            int temp = atoi(addr) - 120;
            sprintf(addr, "%d", temp);
            setNatRow(id, eth, addr, DIP_3, DIP_2, DIP_1, DIP_0, SIP_3, SIP_2, SIP_1, SIP_0);
        }
        
    }
   
    sqlite3_close(db);
    return 0;
}