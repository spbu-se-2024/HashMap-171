#ifndef HASHMAP_171_MULTISET_ERR_H
#define HASHMAP_171_MULTISET_ERR_H

#include <stdio.h>

typedef enum {
    MULTISET_E_OK,
    MULTISET_E_MEM_ALLOC,
    MULTISET_E_NULL_THIS,
    MULTISET_E_NULL_ARG,
    MULTISET_E_OTHER,
} MultisetErrCode;

// TODO : Add appropriate messages for error codes
static char *MULTISET_ERR_MESSAGES[] = {};


void Multiset_printErr(FILE *errStream, MultisetErrCode errCode);

#endif // HASHMAP_171_MULTISET_ERR_H
