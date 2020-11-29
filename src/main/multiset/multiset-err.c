#include "multiset-err.h"

void Multiset_printErr(FILE *errStream, MultisetErrCode errCode) {
    fprintf(errStream, "Multiset - Error: %s\n", MULTISET_ERR_MESSAGES[errCode]);
}
