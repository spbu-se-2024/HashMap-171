#include "hash-function.h"

#include <stdint.h>

#define MULT_PRIME 257 // A const prime by which chars are multiplied

HashFuncErrCode calculatePolynomialHash(const char *message, size_t size, uint64_t *hash) {
    if (message == NULL || hash == NULL) {
        return HASH_FUNC_E_NULL_ARG;
    }
    uint64_t tempHash = 0;
    for (size_t i = 0; i < size; i++) {
        tempHash = tempHash * MULT_PRIME + message[i];
    }
    *hash = tempHash;
    return HASH_FUNC_E_OK;
}

#undef MULT_PRIME
