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
        getMacSwitch();
    }
    //If the web is changing mac innerswitch switch to open or close
    if(strcmp(subRouter, "innerswitch/true") == 0 || strcmp(subRouter, "innerswitch/false") == 0 && strcmp(method, "POST") == 0){
        setMacSwitch("1", getCharPos(uri, "/", 5));
    }

    //If the web is changing mac outerswitch switch to open or close
    if(strcmp(subRouter, "outerswitch/true") == 0 || strcmp(subRouter, "outerswitch/false") == 0 && strcmp(method, "POST") == 0){
        setMacSwitch("2", getCharPos(uri, "/", 5));
    }

    //If the web is getting maclist in database
    if(strcmp(subRouter, "getMacList") == 0 && strcmp(method, "GET") == 0){
        getMac();
    }

    //If the web is setting maclist in database
    if(strcmp(subRouter, "setMacRow") == 0 && strcmp(method, "POST") == 0){
        // int i;
        // char jSON_Name[9][10] = {"id", "eth", "addr", "mac_5", "mac_4", "mac_3", "mac_2", "mac_1", "mac_0"};

        // usr_JSON_array* macJSON_array;

        // macJSON_array -> inputStr = malloc(100);
        // macJSON_array -> inputStr = getCgiData(stdin, method);
        // macJSON_array -> argc = 9;

        // for(i = 0; i < 9; i++){
        //     macJSON_array -> argv[i] = malloc(10);
        //     macJSON_array -> objName[i] = malloc(10);
        //     memset(macJSON_array -> argv[i], 0 ,10);
        //     memset(macJSON_array -> objName[i], 0 ,10);
        //     macJSON_array -> objName[i] = jSON_Name[i];
        // }

        // testgetJSON(macJSON_array);
        // setMacRow(macJSON_array -> argv[0], //id
        //     macJSON_array -> argv[1],       //eth
        //     macJSON_array -> argv[2],       //addr
        //     macJSON_array -> argv[3],       //mac_5
        //     macJSON_array -> argv[4],       //mac_4
        //     macJSON_array -> argv[5],       //mac_3
        //     macJSON_array -> argv[6],       //mac_2     
        //     macJSON_array -> argv[7],       //mac_1
        //     macJSON_array -> argv[8]        //mac_0
        // );
        char id[10], eth[10], addr[10], mac_5[10], mac_4[10], mac_3[10], mac_2[10], mac_1[10], mac_0[10];
        char* input = malloc(100);
        memset(input, 0 ,100);
        memset(id, 0 ,10);
        memset(eth, 0 ,10);
        memset(addr, 0 ,10);
        memset(mac_5, 0 ,10);
        memset(mac_4, 0 ,10);
        memset(mac_3, 0 ,10);
        memset(mac_2, 0 ,10);
        memset(mac_1, 0 ,10);
        memset(mac_0, 0 ,10);

        input = getCgiData(stdin, method);  
   
        getMacJSON(input, id, eth, addr, mac_5, mac_4, mac_3, mac_2, mac_1, mac_0);
        printf("input = %s ", input);
        printf("id = %s ", id);
        printf("eth = %s ", eth);
        printf("addr = %s ", addr);
        printf("mac_5 = %s ", mac_5);
        printf("mac_4 = %s ", mac_4);
        printf("mac_3 = %s ", mac_3);
        printf("mac_2 = %s ", mac_2);
        printf("mac_1 = %s ", mac_1);
        printf("mac_0 = %s ", mac_0);

        setMacRow(id, eth, addr, mac_5, mac_4, mac_3, mac_2, mac_1, mac_0);
    }
   
    sqlite3_close(db);
    return 0;
}