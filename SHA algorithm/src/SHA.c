//
// Created by trefi on 20.11.2020.
//

#include "SHA.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))

unsigned char* formatMessageToBlocks(char* message, unsigned long long size) {
    unsigned int lastBlockSize = size % 64;
    unsigned int lastBlockOffset = size / 64;
    unsigned int blocksCount;
    unsigned long long sizeInBits = size * 8;
    unsigned char* buffer;
    if(lastBlockSize < 56) {
        blocksCount = lastBlockOffset + 1;
    }
    else {
        blocksCount = lastBlockOffset + 2;
    }
    buffer = (unsigned char*)calloc(blocksCount, 64);
    if(buffer == NULL) {
        error("Bad memory alloc in formatMessageToBlocks");
        return NULL;
    }
    for(unsigned i = 0; i < lastBlockOffset; i++) {
        memcpy(buffer + i * 64, message + i * 64, 64);
    }
    unsigned char terminator = 128;
    if(lastBlockSize < 56) {
        for(unsigned int i = 0; i < 56; i++) {
            if(i < lastBlockSize)
                memcpy(buffer + lastBlockOffset * 64 + i, message + lastBlockOffset + i, 1);
            else if(i == lastBlockSize)
                memcpy(buffer + lastBlockOffset * 64 + i, &terminator, 1);
        }
        for(unsigned i = 0; i < 8; i++) {
            memcpy(buffer + lastBlockOffset * 64 + i + 56, (unsigned char*)(&sizeInBits) + 8 - i - 1, 1);
        }
    }
    else if(lastBlockSize >= 56) {
        for(unsigned int i = 0; i < 64; i++) {
            if(i < lastBlockSize)
                memcpy(buffer + lastBlockOffset * 64 + i, message + lastBlockOffset * 64 + i, 1);
            else if(i == lastBlockSize)
                memcpy(buffer + lastBlockOffset * 64 + i, &terminator, 1);
        }
        for(unsigned int i = 0; i < 8; i++) {
            memcpy(buffer + lastBlockOffset * 64 + i + 120, (unsigned char*)(&sizeInBits) + 8 - i - 1, 1);
        }
    }

    return buffer;
}

void SHA_1(char* message, unsigned long long size, unsigned int* hash) { //TODO можно переделать чтобы выделялось меньше памяти, и выделялось на стеке
    unsigned char* buffer = formatMessageToBlocks(message, size);
    if(buffer == NULL) {
        return;
    }

    hash[0] = 0x67452301;
    hash[1] = 0xEFCDAB89;
    hash[2] = 0x98BADCFE;
    hash[3] = 0x10325476;
    hash[4] = 0xC3D2E1F0;

    unsigned int blocksCount = size % 64 < 56 ? size / 64 + 1 : size / 64 + 2;

    for(unsigned int i = 0; i < blocksCount; i++) {
        unsigned int a = hash[0];
        unsigned int b = hash[1];
        unsigned int c = hash[2];
        unsigned int d = hash[3];
        unsigned int e = hash[4];
        unsigned int w[80] = {0};
        unsigned int offset = 64 * i;
        for(unsigned int j = 0; j < 16; j++) {
            w[j] = ((unsigned int)buffer[offset + j * 4 + 3] & 0xFF)
                    | ((unsigned int)buffer[offset + j * 4 + 2] & 0xFF) << 8
                    | ((unsigned int)buffer[offset + j * 4 + 1] & 0xFF) << 16
                    | ((unsigned int)buffer[offset + j * 4] & 0xFF) << 24;
        }
        for(unsigned int j = 16; j < 80; j++) {
            w[j] = w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16];
            w[j] = (w[j] << 1) | (w[j] >> 31); // leftrotate 1
        }
        for(unsigned int j = 0; j < 80; j++) {
            unsigned int f, k;
            if(j < 20) {
                f = (b & c) | (~b & d);
                k = 0x5A827999;
            }
            else if(j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if(j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            unsigned int tmp = (a << 5) | (a >> 27);
            tmp = tmp + f + e + k + w[j];
            e = d;
            d = c;
            c = (b << 30) | (b >> 2);
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
}