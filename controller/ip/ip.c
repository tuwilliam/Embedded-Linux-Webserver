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
        getIpSwitch();
    }

    //If the web is changing inner Port switch switch to open or close
    if(strcmp(subRouter, "innerSwitch/true") == 0 || strcmp(subRouter, "innerSwitch/false") == 0 && strcmp(method, "POST") == 0){
        setIpSwitch("inner", getCharPos(uri, "/", 5));
    }

    //If the web is changing outer Port switch switch to open or close
    if(strcmp(subRouter, "outerSwitch/true") == 0 || strcmp(subRouter, "outerSwitch/false") == 0 && strcmp(method, "POST") == 0){
        setIpSwitch("outer",getCharPos(uri, "/", 5));
    }

    //If the web is getting Iplist in database
    if(strcmp(subRouter, "getIpList") == 0 && strcmp(method, "GET") == 0){
        getIp();
    }

    

    //If the web is setting Iplist in database
    if(strcmp(subRouter, "setIpRow") == 0 && strcmp(method, "POST") == 0){
        printf("   setIpRow  ");
        char id[10], eth[10], addr[10], IP_3[10], IP_2[10], IP_1[10], IP_0[10];
        char* input = malloc(100);
        memset(input, 0 ,100);
        memset(id, 0 ,10);
        memset(eth, 0 ,10);
        memset(addr, 0 ,10);
        memset(IP_3, 0 ,10);
        memset(IP_2, 0 ,10);
        memset(IP_1, 0 ,10);
        memset(IP_0, 0 ,10);

        input = getCgiData(stdin, method);  
   
        getIpJSON(input, id, eth, addr, IP_3, IP_2, IP_1, IP_0);

        setIpRow(id, eth, addr, IP_3, IP_2, IP_1, IP_0);
    }
   
    sqlite3_close(db);
    return 0;
}