#ifndef HASHMAP_171_HASH_FUNCTION_ERR_H
#define HASHMAP_171_HASH_FUNCTION_ERR_H

#include <stdio.h>


typedef enum {
    HASH_FUNC_E_OK,
    HASH_FUNC_E_NULL_ARG,
    HASH_FUNC_E_MEM_ALLOC,
    HASH_FUNC_E_OTHER,
} HashFuncErrCode;

extern const char *const HASH_FUNC_ERR_MESSAGES[];


void HashFunc_printErr(FILE *errStream, HashFuncErrCode errCode);


#endif // HASHMAP_171_HASH_FUNCTION_ERR_H
