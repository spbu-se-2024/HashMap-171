#include "hash-function.h"

#include <stdint.h>


// A const prime by which chars are multiplied
#define MULT_PRIME 257

HashFuncErrCode HashFunc_calcPolynomialHash(const char *message, size_t size, uint64_t *hash) {
    HashFunc_autoprintErrAndStopRunIf(message == NULL, HASH_FUNC_E_NULL_ARG,);
    HashFunc_autoprintErrAndStopRunIf(hash == NULL, HASH_FUNC_E_NULL_ARG,);


    uint64_t tempHash = 0;
    for (size_t i = 0; i < size; i++) {
        tempHash = tempHash * MULT_PRIME + (unsigned char) message[i];
    }

    *hash = tempHash;

    return HASH_FUNC_E_OK;
}

#undef MULT_PRIME
