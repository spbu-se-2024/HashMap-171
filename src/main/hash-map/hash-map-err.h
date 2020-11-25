#ifndef HASHMAP_171_HASH_MAP_ERR_H
#define HASHMAP_171_HASH_MAP_ERR_H

#include <stdio.h>

typedef enum {
    HASH_MAP_E_OK,
    HASH_MAP_E_MEM_ALLOC,
    HASH_MAP_E_NULL_THIS,
    HASH_MAP_E_NULL_ARG,
    HASH_MAP_E_OTHER,
} HashMapErrCode;

// TODO : Add appropriate messages for error codes
static char *HASH_MAP_ERR_MESSAGES[] = {};


void HashMap_printErr(FILE *errStream, HashMapErrCode errCode);

#endif // HASHMAP_171_HASH_MAP_ERR_H
