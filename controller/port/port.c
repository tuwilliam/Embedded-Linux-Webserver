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
        getPortSwitch();
    }

    //If the web is changing inner Port switch switch to open or close
    if(strcmp(subRouter, "innerSwitch/true") == 0 || strcmp(subRouter, "innerSwitch/false") == 0 && strcmp(method, "POST") == 0){
        setPortSwitch("inner", getCharPos(uri, "/", 5));
    }

    //If the web is changing outer Port switch switch to open or close
    if(strcmp(subRouter, "outerSwitch/true") == 0 || strcmp(subRouter, "outerSwitch/false") == 0 && strcmp(method, "POST") == 0){
        setPortSwitch("outer",getCharPos(uri, "/", 5));
    }

    //If the web is getting Portlist in database
    if(strcmp(subRouter, "getPortList") == 0 && strcmp(method, "GET") == 0){
        getPort();
    }

    //If the web is setting Portlist in database
    if(strcmp(subRouter, "setPortRow") == 0 && strcmp(method, "POST") == 0){
        printf("   setPortRow  ");
        char id[10], eth[10], addr[10], port_h[10], port_l[10], protocol[20];
        char* input = malloc(200);
        memset(input, 0 ,200);
        memset(id, 0 ,10);
        memset(eth, 0 ,10);
        memset(addr, 0 ,10);
        memset(port_h, 0 ,10);
        memset(port_l, 0 ,10);
        memset(protocol, 0 ,20);
 
        input = getCgiData(stdin, method);  
        //printf("input = %s ", input);
        
        getPortJSON(input, id, eth, addr, port_h, port_l, protocol);
        
        // printf("id = %s ", id);
        // printf("eth = %s ", eth);
        // printf("addr = %s ", addr);
        // printf("port_h = %s ", port_h);
        // printf("port_l = %s ", port_l);
        // printf("protocol = %s ", protocol);


        setPortRow(id, eth, addr, port_h, port_l, protocol);      
    }
   
    sqlite3_close(db);
    return 0;
}