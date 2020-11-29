#ifndef HASHMAP_171_HASH_FUNCTION_H
#define HASHMAP_171_HASH_FUNCTION_H

#include <stddef.h>

typedef enum {
    HASH_FUNC_E_OK,
    HASH_FUNC_E_MEM_ALLOC,
} HashFuncErrCode;

typedef HashFuncErrCode (*HashFunction)(const char *message, size_t size, unsigned int *hash);

HashFuncErrCode SHA_1(const char *message, size_t size, unsigned int *hash);

HashFuncErrCode MD5(const char *message, size_t size, unsigned int *hash);

HashFuncErrCode polynomial(const char *message, size_t size, unsigned int *hash);

#endif // HASHMAP_171_HASH_FUNCTION_H
