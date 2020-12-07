#ifndef HASHMAP_171_AVL_TREE_ERR_H
#define HASHMAP_171_AVL_TREE_ERR_H

#include "avl-tree-config.h"
#include "../error-handling/error-handling.h"


/*----------------------------------------------- AVL Tree Error Codes -----------------------------------------------*/

typedef enum {
    AVL_TREE_E_OK,
    AVL_TREE_E_MEM_ALLOC,
    AVL_TREE_E_NULL_THIS,
    AVL_TREE_E_NULL_ARG,
    AVL_TREE_E_OTHER,
} AvlTreeErrCode;

extern const char *const AVL_TREE_ERR_MESSAGES[];


/*----------------------------------------- AVL Tree Error Handling Interface ----------------------------------------*/

#define AvlTree_printErr(errStream, errCode) \
    ErrorHandling_printErr(                  \
        AvlTree, AVL_TREE, "AVL Tree"        \
        errStream, errCode                   \
    )

#define AvlTree_autoprintErr(errCode)                  \
    do {                                               \
        if (AVL_TREE_OPT_1_AUTOPRINT_ERR) {            \
            ErrorHandling_printErr(                    \
                AvlTree, AVL_TREE, "AVL Tree",         \
                AVL_TREE_OPT_1_ARG_ERR_STREAM, errCode \
            );                                         \
        }                                              \
    } while(0)


#define AvlTree_stopRunOnBadErrCode(errCode, codeToRunOnBadErrCode) \
    ErrorHandling_stopRunOnBadErrCode(                              \
        AvlTree, AVL_TREE, "AVL Tree",                              \
        errCode, codeToRunOnBadErrCode                              \
    )


#define AvlTree_printErrAndStopRunIf(isErr, errStream, errCode, codeToRunIfIsErr) \
    ErrorHandling_printErrAndStopRunIf(                                           \
        AvlTree, AVL_TREE, "AVL Tree"                                             \
        isErr, errStream, errCode, codeToRunIfIsErr                               \
    )

#define AvlTree_autoprintErrAndStopRunIf(isErr, errCode, codeToRunIfIsErr)      \
    do {                                                                        \
        if (AVL_TREE_OPT_1_AUTOPRINT_ERR) {                                     \
            ErrorHandling_printErrAndStopRunIf(                                 \
                AvlTree, AVL_TREE, "AVL Tree",                                  \
                isErr, AVL_TREE_OPT_1_ARG_ERR_STREAM, errCode, codeToRunIfIsErr \
            );                                                                  \
        }                                                                       \
    } while(0)


#endif // HASHMAP_171_AVL_TREE_ERR_H
