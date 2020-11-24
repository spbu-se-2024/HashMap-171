#ifndef HASHMAP_171_HASH_FUNCTION_H
#define HASHMAP_171_HASH_FUNCTION_H

#include <stddef.h>

typedef void (*HashFunction)(const char *message, size_t size, unsigned long long *hash);

#endif // HASHMAP_171_HASH_FUNCTION_H
