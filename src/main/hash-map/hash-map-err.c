#include "hash-map-err.h"

void HashMap_printErr(FILE *errStream, HashMapErrCode errCode) {
    fprintf(errStream, "HashMap - Error: %s\n", HASH_MAP_ERR_MESSAGES[errCode]);
}
