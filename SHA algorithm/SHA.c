//
// Created by trefi on 20.11.2020.
//

#include "SHA.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))

unsigned char* format_message_to_blocks(char* message, unsigned long long size) {
    unsigned int last_block_size = size % 64;
    unsigned int last_block_offset = size / 64;
    unsigned int blocks_count;
    unsigned long long size_in_bits = size * 8;
    unsigned char* buffer;
    if(last_block_size < 56) {
        blocks_count = last_block_offset + 1;
    }
    else {
        blocks_count = last_block_offset + 2;
    }
    buffer = (unsigned char*)calloc(blocks_count, 64);
    if(buffer == NULL) {
        error("Bad memory alloc in format_message_to_blocks");
        return NULL;
    }
    for(unsigned i = 0; i < last_block_offset; i++) {
        memcpy(buffer + i * 64, message + i * 64, 64);
    }
    unsigned char terminator = 128;
    if(last_block_size < 56) {
        for(unsigned int i = 0; i < 56; i++) {
            if(i < last_block_size)
                memcpy(buffer + last_block_offset * 64 + i, message + last_block_offset + i, 1);
            else if(i == last_block_size)
                memcpy(buffer + last_block_offset * 64 + i, &terminator, 1);
        }
        for(unsigned i = 0; i < 8; i++) {
            memcpy(buffer + last_block_offset * 64 + i + 56, (unsigned char*)(&size_in_bits) + 8 - i - 1, 1);
        }
    }
    else if(last_block_size >= 56) {
        for(unsigned int i = 0; i < 64; i++) {
            if(i < last_block_size)
                memcpy(buffer + last_block_offset * 64 + i, message + last_block_offset * 64 + i, 1);
            else if(i == last_block_size)
                memcpy(buffer + last_block_offset * 64 + i, &terminator, 1);
        }
        for(unsigned int i = 0; i < 8; i++) {
            memcpy(buffer + last_block_offset * 64 + i + 120, (unsigned char*)(&size_in_bits) + 8 - i - 1, 1);
        }
    }

    return buffer;
}

void SHA_1(char* message, unsigned long long size, unsigned int* out_hash) {
    unsigned char* buffer = format_message_to_blocks(message, size);
    if(buffer == NULL) {
        return;
    }

    out_hash[0] = 0x67452301;
    out_hash[1] = 0xEFCDAB89;
    out_hash[2] = 0x98BADCFE;
    out_hash[3] = 0x10325476;
    out_hash[4] = 0xC3D2E1F0;

    unsigned int blocks_count = size % 64 < 56 ? size / 64 + 1 : size / 64 + 2;

    for(unsigned int i = 0; i < blocks_count; i++) {
        unsigned int a = out_hash[0];
        unsigned int b = out_hash[1];
        unsigned int c = out_hash[2];
        unsigned int d = out_hash[3];
        unsigned int e = out_hash[4];
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
        out_hash[0] += a;
        out_hash[1] += b;
        out_hash[2] += c;
        out_hash[3] += d;
        out_hash[4] += e;
    }

    free(buffer);
}