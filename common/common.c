#include "common.h"

/**
*Get cgi content from web front-end
*@parameter fp is the standard input, it's a file handle
*@parameter method is the cgi method from web front-end
*@return the cgi content string
*@author SUN ZHOGNJIAN
*/
char* getCgiData(FILE* fp, char* method){
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

/**
*Get a string's position at the Nth time occurrence of the source string
*@parameter scr is the source string
*@parameter search is the string need to be searched
*@parameter pos is the search times
*@return the search result
*@author SUN ZHOGNJIAN
*/
char* getCharPos(char* scr, char* search, int pos){

	int i;
	char* temp = scr;

	for(i = 0; i < pos ;i++){
		temp = strstr(temp, search) + 1;
	}

	return temp;
}