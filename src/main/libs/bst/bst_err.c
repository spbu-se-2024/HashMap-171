#include "bst_err.h"

void Bst_printErr(FILE *errStream, BstErrCode errCode) {
    fprintf(errStream, "BST - Error: %s\n", BST_ERR_MESSAGES[errCode]);
}
