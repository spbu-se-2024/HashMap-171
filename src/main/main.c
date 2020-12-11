#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "multiset/multiset.h"

typedef struct {
    char *filename;
    MultisetConfig config;
} InputData;

#define printErr(errMsg) fprintf(stderr, errMsg)

/*------------------------------------------- Parse Command Line Arguments -------------------------------------------*/

#define EXPECTED_CL_ARGS_NUM 3
#define HASH_FUNCS_NUM 3
#define HASH_FUNCS_NAMES {"md5","polynomial","sha-1"}

unsigned int parseClArgs(InputData *inputData, size_t argc, char **argv) {
    if (argc != EXPECTED_CL_ARGS_NUM + 1) {
        printErr("Unexpected number of arguments\n");
        return 1;
    }

    inputData->filename = argv[1];

    inputData->config.size = strtoul(argv[2], NULL, 10);
    if (inputData->config.size == 0 || !('0' <= argv[2][0] && argv[2][0] <= '9')) {
        printErr("Invalid hash table size given (must be positive)\n");
        return 1;
    }

    const char *hashFuncsNames[HASH_FUNCS_NUM] = HASH_FUNCS_NAMES;
    _Bool isHashFuncGiven = 0;
    for (unsigned int i = MULTISET_HASH_FUNC_LABEL_MD5; i <= MULTISET_HASH_FUNC_LABEL_SHA_1; i++) {
        if (strcmp(argv[3], hashFuncsNames[i - MULTISET_HASH_FUNC_LABEL_MD5]) == 0) {
            inputData->config.hashFuncLabel = i;
            isHashFuncGiven = 1;
        }
    }
    if (!isHashFuncGiven) {
        printErr("Unsupported hash function name given (must be md5, polynomial or sha-1)\n");
        return 1;
    }

    return 0;
}

#undef EXPECTED_CL_ARGS_NUM


/*------------------------------------------------ Read File ---------------------------------------------------------*/

int readFile(Multiset *pTable, char *filename) {
    FILE *file;
    file = fopen(filename, "r");

    char c;
    char word[1000];
    size_t count = 0;
    while ((c = (char) fgetc(file)) != EOF) {
        if (isalpha(c)) {
            word[count++] = c;
        } else {
            if (count != 0) {
                word[count++] = '\0';
                char *item = malloc(count);
                strncpy(item, word, count);
                pTable->addItem(pTable, item);
            }
            count = 0;
        }
    }
    if (count) {
        word[count++] = '\0';
        char *item = malloc(count);
        strncpy(item, word, count);
        pTable->addItem(pTable, item);
    }
    fclose(file);
    return 0;
}

/*-------------------------------------------------- Main ------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    InputData inputData;
    if (parseClArgs(&inputData, argc, argv)) return EXIT_FAILURE;
    const char *hashFuncsNames[HASH_FUNCS_NUM] = HASH_FUNCS_NAMES;

    Multiset hashTable, *pTable = &hashTable;
    if (Multiset_initMultiset(&hashTable, inputData.config) != MULTISET_E_OK) return EXIT_FAILURE;

    clock_t begin = clock();
    readFile(pTable, inputData.filename);
    clock_t end = clock();
    double time = (double) (end - begin) / CLOCKS_PER_SEC;

    MultisetStats stats;
    pTable->getStatistics(pTable, &stats);
    printf("Words number: ......................... %ld\n"
           "Unique by hash words number: .......... %ld\n"
           "The most frequently used word: ........ %s\n"
           "Max number of words with the same hash: %ld\n"
           "Execution time: ....................... %lf\n"
           "Size: ................................. %ld\n"
           "Hash Function: ........................ %s\n",
           stats.itemsCount, stats.uniqueItemsCount, stats.maxCountWord, stats.maxCount, time, inputData.config.size,
           hashFuncsNames[inputData.config.hashFuncLabel]);

    Multiset_eraseMultiset(pTable);


    return EXIT_SUCCESS;
}

#undef HASH_FUNCS_NAMES
#undef HASH_FUNCS_NUM
