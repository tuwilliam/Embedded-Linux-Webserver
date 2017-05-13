#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

typedef struct userData
{
	char* username;
	char* pwd;
}userData;

void getjSON(char* input, char* username, char* pwd);

void makeSession();
char* makeJSON();