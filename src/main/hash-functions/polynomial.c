#include "hash-function.h"

#include <stdint.h>
#include <limits.h>

#define MULT_PRIME 257                                              // A const prime by which chars are multiplied
#define HASH_ELEMENTS_USED (sizeof(uint64_t) / sizeof(uint32_t))    // Number of elements used in hash array (equals 2)
#define UINT32_BIT_SIZE (sizeof(uint32_t) * CHAR_BIT)               // Size of uint32_t in bits (equals 32)
#define UINT32_BIT_MASK ((uint64_t) ((1ul << UINT32_BIT_SIZE) - 1)) // Masks off everything but right-most 32 bits

HashFuncErrCode calculatePolynomialHash(const char *message, size_t size, uint32_t *hash) {
    uint64_t hashBuf = 0;
    for (unsigned int i = 0; i < size; i++) {
        hashBuf = hashBuf * MULT_PRIME + message[i];
    }
    for (unsigned int i = 0; i < HASH_ELEMENTS_USED; i++) {
        hash[HASH_ELEMENTS_USED - 1 - i] = (hashBuf & (UINT32_BIT_MASK << UINT32_BIT_SIZE * i)) >> UINT32_BIT_SIZE * i;
    }
    return HASH_FUNC_E_OK;
}

#undef MULT_PRIME
#undef HASH_ELEMENTS_USED
#undef UINT32_BIT_SIZE
#undef UINT32_BIT_MASK
