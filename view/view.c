#include "../model/session.h"

void renderPage(char* fileName){
    printf("Content-type:text/html\n\n");
	FILE *pFile=fopen(fileName,"r");
    char *pBuf;  //定义文件指针
    fseek(pFile,0,SEEK_END); //把指针移动到文件的结尾 ，获取文件长度
    int len=ftell(pFile); //获取文件长度
    pBuf = (char*)malloc(len+1); //定义数组长度
    rewind(pFile); //把指针移动到文件开头 因为我们一开始把指针移动到结尾，如果不移动回来 会出错
    fread(pBuf,1,len,pFile); //读文件
    pBuf[len]=0; //把读到的文件最后一位 写为0 要不然系统会一直寻找到0后才结束
    fclose(pFile); // 关闭文件
    printf("%s",pBuf);
    free(pBuf);
}


static void renderHomePageFun(int argc, char** argv){
    //TODO
    renderPage(argv[0]);
    
}

static void renderLoginPageFun(int argc, char** argv){
    //TODO
    renderPage(argv[0]);
}

int main(void){

    sqlite3* db;
    int  rc;

	char* input = (char*)malloc(100);
    char* method;

    method = getenv("REQUEST_METHOD");
    input = getCgiData(stdin, method);
    printf("input=%s\n", input);

    int user_accessIDLen = getCharPos(input, "&", 1) - getCharPos(input, "=", 1) - 1;
    char user_accessID[100] = {0};
    strncpy(user_accessID, getCharPos(input, "=", 1), user_accessIDLen);

    char* token = getCharPos(input, "=", 2);

    printf("%s   ", user_accessID);
    printf("%s   ", token);

    SessionCheckFun renderHomePage;
    renderHomePage.func = renderHomePageFun;
    renderHomePage.argc = 1;
    renderHomePage.argv = (char**)malloc(1);
    renderHomePage.argv[0] = (char*)malloc(sizeof("./home.html"));
    renderHomePage.argv[0] = "./home.html";

    SessionCheckFun renderLoginPage;
    renderLoginPage.func = renderLoginPageFun;
    renderLoginPage.argc = 1;
    renderLoginPage.argv = (char**)malloc(1);
    renderLoginPage.argv[0] = (char*)malloc(sizeof("../index.html"));
    renderLoginPage.argv[0] = "../index.html";

    //sessionCheck(renderHomePage, renderLoginPage);
    rc = sqlite3_open("./model/netgap.db", &db);
    //error when open DB
    if(rc){
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(input);
        free(renderHomePage.argv[0]);
        free(renderHomePage.argv);
        exit(1);
    }

    sessionCheck(user_accessID, token, &renderHomePage, &renderLoginPage, db, rc);

    free(input);
    free(renderHomePage.argv[0]);
    free(renderHomePage.argv);
	return 0;
}