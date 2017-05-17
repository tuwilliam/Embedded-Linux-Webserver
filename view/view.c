#include "../common/common.h"

void renderPage(char* fileName){
	FILE *pFile=fopen(fileName,"r");
    char *pBuf;  //定义文件指针
    fseek(pFile,0,SEEK_END); //把指针移动到文件的结尾 ，获取文件长度
    int len=ftell(pFile); //获取文件长度
    pBuf = malloc(len+1); //定义数组长度
    rewind(pFile); //把指针移动到文件开头 因为我们一开始把指针移动到结尾，如果不移动回来 会出错
    fread(pBuf,1,len,pFile); //读文件
    pBuf[len]=0; //把读到的文件最后一位 写为0 要不然系统会一直寻找到0后才结束
    fclose(pFile); // 关闭文件
    printf("%s",pBuf);
    free(pBuf);
}

int main(void){

	printf("Content-type:text/html\n\n");

	char* input = malloc(100);
    char* method;

    method = getenv("REQUEST_METHOD");
    input = getCgiData(stdin, method);
    printf("input=%s\n", input);

    int user_accessIDLen = getCharPos(input, "&", 1) - getCharPos(input, "=", 1) - 1;
    char user_accessID[100] = {0};
    strncpy(user_accessID, getCharPos(input, "=", 1), user_accessIDLen);

    char* token = getCharPos(input, "=", 2);

    printf("%s\n", user_accessID);
    printf("%s\n", token);

    SessionCheckFun* renderHomePage;
    renderHomePage -> func = renderHomePageFun;
    renderHomePage -> argc = 1;
    renderHomePage -> argv[0] = "./home.html";

    SessionCheckFun* renderLoginPage;
    renderLoginPage -> func = renderLoginPageFun;
    renderLoginPage -> argc = 1;
    renderLoginPage -> argv[0] = "./home.html";

    sessionCheck(user_accessID, token, renderHomePage, renderLoginPage, db, rc);
    // if(sessionCheck(user_accessID, token, renderHomePage, renderLoginPage, db, rc){

    // }
    // else{
    // 	renderPage("../index.html");
    // }
    

	return 0;
}

void renderHomePageFun(int argc, char** argv){
    //TODO
    renderPage(argv[0]);
    
}

void renderLoginPageFun(int argc, char** argv){
    //TODO
    renderPage(argv[0]);
}