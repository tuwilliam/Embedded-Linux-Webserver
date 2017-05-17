#include "../common/common.h"

typedef struct sessionCheckFun{
    void* func;
    int argc;
    char** argv;

}SessionCheckFun;

int sessionCheck(char* user_accessID, char* token, sqlite3* db, int rc);