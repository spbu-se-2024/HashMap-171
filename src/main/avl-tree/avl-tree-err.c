#include "avl-tree-err.h"


// TODO : Add appropriate messages for error codes
const char *const AVL_TREE_ERR_MESSAGES[] = {
    "TODO",
};


void AvlTree_printErr(FILE *errStream, AvlTreeErrCode errCode) {
    fprintf(errStream, "AVL Tree - Error: %s\n", AVL_TREE_ERR_MESSAGES[errCode]);
}
