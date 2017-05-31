#include "../../common/common.h"
#include "../../common/APP.h"

int main(int argc,char** argv)  
{  
    int status ,i;  
    int cflags = REG_EXTENDED;  
    regmatch_t pmatch[1];  
    const size_t nmatch = 1;  
    regex_t reg;  
    const char * pattern = "/cgi-bin/view(\\?{1})id=([0-9]+)&token=(\\S+)$";  
    char * buf = "/cgi-bin/view?id=1&token=18782677";  
    regcomp(&reg,pattern,cflags);//编译正则模式  
    status = regexec(&reg,buf,nmatch,pmatch,0);//执行正则表达式和缓存的比较  
    if(status == REG_NOMATCH)  
        printf("No match\n");  
    else if (0 == status)  
    {  
        printf("success\n:");  
        for(i = pmatch[0].rm_so;i<pmatch[0].rm_eo;++i)putchar(buf[i]);  
        printf("\n");  
    }  
    regfree(&reg);  
    return 0;  
}