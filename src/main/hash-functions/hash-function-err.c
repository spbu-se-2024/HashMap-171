#include "hash-function-err.h"


// TODO : Add appropriate messages for error codes
const char *const HASH_FUNC_ERR_MESSAGES[] = {
    "TODO",
};


void HashFunc_printErr(FILE *errStream, HashFuncErrCode errCode) {
    fprintf(errStream, "Hash Function - Error: %s\n", HASH_FUNC_ERR_MESSAGES[errCode]);
}
