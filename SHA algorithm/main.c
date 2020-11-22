#include <stdio.h>
#include "SHA.h"

int main() {

    char string[128] = "hello worldhello worldhello worldhello worldhello worlddddddddddhello worldhello worldhello worldhello worldhello worldddddddddd";
    unsigned int hash[5];

    for(unsigned i = 0; i < 128; i++) {
        printf("%2u : %2.2x\n", i, string[i]);
    }
    printf("\n");

   SHA_1(string, 128, hash);
    for(unsigned i = 0; i < 5; i++) {
        printf("%x ", hash[i]);
    }

    return 0;
}