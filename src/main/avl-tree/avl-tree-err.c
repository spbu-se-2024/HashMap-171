#include "avl-tree-err.h"

void AvlTree_printErr(FILE *errStream, AvlTreeErrCode errCode) {
    fprintf(errStream, "AVL Tree - Error: %s\n", AVL_TREE_ERR_MESSAGES[errCode]);
}
