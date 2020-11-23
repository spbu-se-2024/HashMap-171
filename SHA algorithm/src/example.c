#include <stdio.h>
#include <string.h>
#include "SHA.h"

int main() {

    char string[256];
    unsigned int hash[5];

    printf("Put string in stdin (< 256 bytes)\n");
    fgets(string, 256, stdin);

    SHA_1(string, strlen(string) - 1, hash); //strlen(...) - 1 - вычитание из длины символа LF
    for(unsigned i = 0; i < 5; i++) {
        printf("%x ", hash[i]);
    }

    return 0;
}