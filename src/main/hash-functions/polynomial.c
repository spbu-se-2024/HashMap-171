#include "hash-function.h"

#include <stdint.h>

#define MULT_PRIME 257 // A const prime by which chars are multiplied

HashFuncErrCode calculatePolynomialHash(const char *message, size_t size, uint64_t *hash) {
    if (message == NULL || hash == NULL) {
        return HASH_FUNC_E_NULL_ARG;
    }
    for (size_t i = 0; i < size; i++) {
        *hash = *hash * MULT_PRIME + message[i];
    }
    return HASH_FUNC_E_OK;
}

#undef MULT_PRIME
