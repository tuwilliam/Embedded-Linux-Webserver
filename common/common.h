/*
  every library is here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "sqlite3.h"

/* Returns the string that web front-end to web back-end */
char* getCgiData(FILE* fp, char* method);
/* Returns a string's position at the Nth time occurrence of the source string */
char* getCharPos(char* scr, char* search, int pos);
