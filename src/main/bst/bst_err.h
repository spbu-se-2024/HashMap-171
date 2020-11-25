#ifndef HASHMAP_171_BST_ERR_H
#define HASHMAP_171_BST_ERR_H

#include <stdio.h>

typedef enum {
    BST_E_OK,
    BST_E_MEM_ALLOC,
    BST_E_NULL_THIS,
    BST_E_NULL_ARG,
    BST_E_OTHER,
} BstErrCode;

// TODO : Add appropriate messages for error codes
static char *BST_ERR_MESSAGES[] = {};


void Bst_printErr(FILE *errStream, BstErrCode errCode);

#endif // HASHMAP_171_BST_ERR_H
