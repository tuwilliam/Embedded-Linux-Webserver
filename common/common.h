#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "sqlite3.h"
#include "userdata.h"
#include "../model/session.h"

char* getCgiData(FILE* fp, char* method);
char* getCharPos(char* scr, char* search, int pos);
