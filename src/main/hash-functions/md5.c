#include "hash-function.h"
#include <stdlib.h>
#include <string.h>

// functions
#define LEFT_ROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
#define FUN_F(b, c, d) (((b) & (c)) | ((~b) & (d)))
#define FUN_G(b, c, d) (((b) & (d)) | ((~d) & (c)))
#define FUN_H(b, c, d) ((b) ^ (c) ^ (d))
#define FUN_I(b, c, d) ((c) ^ ((~d) | (b)))

HashFuncErrCode calculateMd5Hash(const char *message, size_t size, uint32_t *hash) {
    if(message == NULL || hash == NULL) return HASH_FUNC_E_NULL_ARG;

    // Pre-processing
    size_t newSize = (((size + 8) / 64) + 1) * 64;
    uint8_t *newMsg = calloc(newSize + 64, 1);
    if (newMsg == NULL) return HASH_FUNC_E_MEM_ALLOC;
    memcpy(newMsg, message, size);
    newMsg[size] = 128;
    size_t bitSize = 8 * size;
    memcpy(newMsg + newSize - 8, &bitSize, 8);

    // Initialize variables
    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;

    static const uint32_t s[] =
            {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
             5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
             4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
             6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

    static const uint32_t k[] =
            {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
             0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
             0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
             0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
             0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
             0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
             0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
             0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
             0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
             0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
             0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
             0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
             0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
             0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
             0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
             0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

    // Main loop
    for (size_t offset = 0; offset < newSize; offset += 64) {
        uint32_t *words = (uint32_t *) (newMsg + offset);

        uint32_t a = a0;
        uint32_t b = b0;
        uint32_t c = c0;
        uint32_t d = d0;

        for (int i = 0; i < 64; i++) {
            uint32_t f, g;

            if (i < 16) {
                f = FUN_F(b, c, d);
                g = i;
            } else if (i < 32) {
                f = FUN_G(b, c, d);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = FUN_H(b, c, d);
                g = (3 * i + 5) % 16;
            } else {
                f = FUN_I(b, c, d);
                g = (7 * i) % 16;
            }
            f += a + k[i] + words[g];
            a = d;
            d = c;
            c = b;
            b += LEFT_ROTATE(f, s[i]);
        }
        a0 += a;
        b0 += b;
        c0 += c;
        d0 += d;
    }

    // uint32_t hash[4] BE
    uint8_t *p;
    p = (uint8_t *)&a0;
    hash[0] = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
    printf("%x ", hash[0]);
    p = (uint8_t *)&b0;
    hash[1] = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
    p = (uint8_t *)&c0;
    hash[2] = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
    p = (uint8_t *)&d0;
    hash[3] = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];

    /* sample
    uint32_t hash[4] = {0};
    if (!MD5("", 0, hash)){
        for (int i = 0; i < 4; i++)
            printf("%x", hash[i]);
        puts(""); // d41d8cd98f00b204e9800998ecf8427e
    }
    */

    free(newMsg);

    return HASH_FUNC_E_OK;
}

#undef LEFT_ROTATE
#undef FUN_F
#undef FUN_G
#undef FUN_H
#undef FUN_I
