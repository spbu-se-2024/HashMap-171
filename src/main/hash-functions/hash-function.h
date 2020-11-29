#ifndef HASHMAP_171_HASH_FUNCTION_H
#define HASHMAP_171_HASH_FUNCTION_H

#include <stddef.h>

enum HashError {
    OK,
    ALLOCATION_ERROR
};

typedef enum HashError (*HashFunction)(const char *message, size_t size, unsigned int *hash);

enum HashError SHA_1(const char *message, size_t size, unsigned int *hash);

enum HashError polynomial(const char *message, size_t size, unsigned int *hash);

#endif // HASHMAP_171_HASH_FUNCTION_H
