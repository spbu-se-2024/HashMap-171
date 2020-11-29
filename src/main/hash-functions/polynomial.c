#include <stdint.h>

#include "hash-function.h"

#define CEIL_DIV(x, y) (1 + (x - 1) / y)

#define MULT_PRIME 257 // A const prime by which chars are multiplied
#define UINT_BIT_MASK ((uint64_t) ((2ul << (sizeof(unsigned int) * 8)) - 1ul)) // Masks off everything but num of bits needed to store an unsigned int

enum HashError polynomial(const char *message, size_t size, unsigned int *hash) {
    uint64_t hashBuf = 0;
    for (unsigned int i = 0; i < size; i++) {
        hashBuf = hashBuf * MULT_PRIME + message[i];
    }
    const size_t blocksNum = CEIL_DIV(sizeof(uint64_t), sizeof(unsigned int));
    for (unsigned int i = 0; i < blocksNum; i++) {
        hash[blocksNum - 1 - i] = hashBuf & (UINT_BIT_MASK << sizeof(unsigned int) * 8 * i);
    }
    return OK;
}
