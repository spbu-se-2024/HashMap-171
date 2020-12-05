#ifndef HASHMAP_171_HASH_FUNCTION_H
#define HASHMAP_171_HASH_FUNCTION_H

#include "hash-function-err.h"

#include <stddef.h>
#include <stdint.h>


HashFuncErrCode calculateMd5Hash(const char *message, size_t size, uint32_t *hash);

HashFuncErrCode calculatePolynomialHash(const char *message, size_t size, uint32_t *hash);

HashFuncErrCode calculateSha1Hash(const char *message, size_t size, uint32_t *hash);


#endif // HASHMAP_171_HASH_FUNCTION_H
