#include "hash-function.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

static uint8_t *formatMessageToBlocks(const char *message, size_t size) {
    size_t lastBlockSize = size % 64;
    size_t lastBlockOffset = size / 64;
    size_t blocksCount;
    size_t sizeInBits = size * 8;
    uint8_t *buffer;
    if (lastBlockSize < 56) {
        blocksCount = lastBlockOffset + 1;
    } else {
        blocksCount = lastBlockOffset + 2;
    }
    buffer = (uint8_t *) calloc(blocksCount, 64);
    if (buffer == NULL) {
        return NULL;
    }
    for (unsigned int i = 0; i < lastBlockOffset; i++) {
        memcpy(buffer + i * 64, message + i * 64, 64);
    }
    uint8_t terminator = 128;
    if (lastBlockSize < 56) {
        for (unsigned int i = 0; i < 56; i++) {
            if (i < lastBlockSize)
                memcpy(buffer + lastBlockOffset * 64 + i, message + lastBlockOffset + i, 1);
            else if (i == lastBlockSize)
                memcpy(buffer + lastBlockOffset * 64 + i, &terminator, 1);
        }
        for (unsigned int i = 0; i < 8; i++) {
            memcpy(buffer + lastBlockOffset * 64 + i + 56, (uint8_t *) (&sizeInBits) + 8 - i - 1, 1);
        }
    } else if (lastBlockSize >= 56) {
        for (unsigned int i = 0; i < 64; i++) {
            if (i < lastBlockSize)
                memcpy(buffer + lastBlockOffset * 64 + i, message + lastBlockOffset * 64 + i, 1);
            else if (i == lastBlockSize)
                memcpy(buffer + lastBlockOffset * 64 + i, &terminator, 1);
        }
        for (unsigned int i = 0; i < 8; i++) {
            memcpy(buffer + lastBlockOffset * 64 + i + 120, (uint8_t *) (&sizeInBits) + 8 - i - 1, 1);
        }
    }

    return buffer;
}

// TODO : Can be redone to allocate less memory, and allocate on the stack
HashFuncErrCode calculateSha1Hash(const char *message, size_t size, uint32_t *hash) {
    if(message == NULL) return HASH_FUNC_E_NULL_ARG;
    if(hash == NULL) return HASH_FUNC_E_NULL_ARG;

    uint8_t *buffer = formatMessageToBlocks(message, size);
    if (buffer == NULL) return HASH_FUNC_E_MEM_ALLOC;

    hash[0] = 0x67452301;
    hash[1] = 0xEFCDAB89;
    hash[2] = 0x98BADCFE;
    hash[3] = 0x10325476;
    hash[4] = 0xC3D2E1F0;

    size_t blocksCount = size % 64 < 56 ? size / 64 + 1 : size / 64 + 2;

    for (unsigned int i = 0; i < blocksCount; i++) {
        uint32_t a = hash[0];
        uint32_t b = hash[1];
        uint32_t c = hash[2];
        uint32_t d = hash[3];
        uint32_t e = hash[4];
        uint32_t w[80] = {0};
        unsigned int offset = 64 * i;
        for (unsigned int j = 0; j < 16; j++) {
            w[j] = (buffer[offset + j * 4 + 3] & 0xFFu)
                   | (buffer[offset + j * 4 + 2] & 0xFFu) << 8u
                   | (buffer[offset + j * 4 + 1] & 0xFFu) << 16u
                   | (buffer[offset + j * 4] & 0xFFu) << 24u;
        }
        for (unsigned int j = 16; j < 80; j++) {
            w[j] = w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16];
            w[j] = (w[j] << 1u) | (w[j] >> 31u); // leftrotate 1
        }
        for (unsigned int j = 0; j < 80; j++) {
            uint32_t f, k;
            if (j < 20) {
                f = (b & c) | (~b & d);
                k = 0x5A827999;
            } else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            uint32_t tmp = (a << 5u) | (a >> 27u);
            tmp = tmp + f + e + k + w[j];
            e = d;
            d = c;
            c = (b << 30u) | (b >> 2u);
            b = a;
            a = tmp;
        }
        hash[0] += a;
        hash[1] += b;
        hash[2] += c;
        hash[3] += d;
        hash[4] += e;
    }

    free(buffer);

    return HASH_FUNC_E_OK;
}
