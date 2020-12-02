#ifndef HASHMAP_171_AVL_TREE_ERR_H
#define HASHMAP_171_AVL_TREE_ERR_H

#include <stdio.h>


typedef enum {
    AVL_TREE_E_OK,
    AVL_TREE_E_MEM_ALLOC,
    AVL_TREE_E_NULL_THIS,
    AVL_TREE_E_NULL_ARG,
    AVL_TREE_E_OTHER,
} AvlTreeErrCode;

extern const char *const AVL_TREE_ERR_MESSAGES[];


void AvlTree_printErr(FILE *errStream, AvlTreeErrCode errCode);


#endif // HASHMAP_171_AVL_TREE_ERR_H
