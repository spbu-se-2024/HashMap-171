#include "multiset-err.h"


// TODO : Add appropriate messages for error codes
const char *const MULTISET_ERR_MESSAGES[] = {
    "TODO",
};


void Multiset_printErr(FILE *errStream, MultisetErrCode errCode) {
    fprintf(errStream, "Multiset - Error: %s\n", MULTISET_ERR_MESSAGES[errCode]);
}
