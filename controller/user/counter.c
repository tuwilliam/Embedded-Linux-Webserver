#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
//#include <iostream>
//using namespace std;

char* get_cgi_data(FILE* fp, char* method){
	char* input;
	int len;
	int size=1024;
	int i=0;

	/**< GET method */
	if (strcmp(method, "GET") == 0){ 
			input = getenv("QUERY_STRING");
			return input;
	}
	/**< POST method */
	else if (strcmp(method, "POST") == 0){ 
			len = atoi(getenv("CONTENT_LENGTH"));
			input = (char*)malloc(sizeof(char) * (size+1));
			if (len == 0){
					input[0] = '\0';
					return input;
				}
			while (1){
				input[i] = (char)fgetc(fp);
				if (i == size){
						input[i+1] = '\0';
						return input;
					}
				--len;
				if (feof(fp) || (!(len))){
						i++;
						input[i] = '\0';
						return input;
					}
				i++;
			}
		}
	return NULL;
}


int get_jSON(char* input){
    cJSON *json , *json_num1 , *json_num2; 
    int num1,num2;
    // 解析数据包  
    json = cJSON_Parse(input);  
    if (!json)  
    {  
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());  
    }  
    else  
    {  
        // 解析开关值  
        json_num1 = cJSON_GetObjectItem( json , "num1");  
		
        // printf("json_num1 is: %s", json_num1 -> valuestring);
    	num1 = atoi(json_num1 -> valuestring);
    	// printf("num1 is: %d", num1);
        //else{
        //	if(json_num1->type == cJSON_String){
        //		num1 = 
        //	}
        //}

        json_num2 = cJSON_GetObjectItem( json , "num2");  
        
        // printf("    json_num2 is: %s", json_num2 -> valuestring);
		num2 = atoi(json_num2 -> valuestring);
        //printf("num2 is: %d", num2);

        cJSON_Delete(json);  
        return num1 + num2;
    }  
}

int main(void)
{
	char* input;
	char* method;
	int  number1;
	int  number2;

	printf("Content-type:text/html\n\n");
	//printf("The following is query result:");
	method = getenv("REQUEST_METHOD");
	printf("%s",getenv("REMOTE_ADDR"));
	printf("%s",getenv("CONTENT_TYPE"));
	printf("%s",getenv("REQUEST_URI"));
	input = get_cgi_data(stdin, method);
	//printf("input is: %s", input);
	//get_jSON(input);
	printf("result is: %d", get_jSON(input));
	return 0;
}