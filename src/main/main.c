#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "multiset/multiset.h"

typedef struct {
    char *filename;
    MultisetConfig config;
} InputData;

/*------------------------------------------- Parse Command Line Arguments -------------------------------------------*/

#define EXPECTED_CL_ARGS_NUM 3
#define HASH_FUNCS_NUM 3
#define HASH_FUNCS_NAMES {"md5","polynomial","sha-1"}

int parseClArgs(InputData *inputData, size_t argc, char **argv) {
    if (argc != EXPECTED_CL_ARGS_NUM + 1) return 1; // Unexpected argv number

    inputData->filename = argv[1];

    inputData->config.size = strtoul(argv[2], NULL, 10);
    if (inputData->config.size == 0 || !('0' <= argv[2][0] && argv[2][0] <= '9')) return 2; // Unsupported size

    const char* hashFuncsNames[HASH_FUNCS_NUM] = HASH_FUNCS_NAMES;
    _Bool isHashFuncGiven = 0;
    for (unsigned int i = MULTISET_HASH_FUNC_LABEL_MD5; i <= MULTISET_HASH_FUNC_LABEL_SHA_1; i++) {
        if (strcmp(argv[3], hashFuncsNames[i - MULTISET_HASH_FUNC_LABEL_MD5]) == 0) {
            inputData->config.hashFuncLabel = i;
            isHashFuncGiven = 1;
        }
    }
    if (!isHashFuncGiven) return 3; // Unsupported hash function

    return 0;
}

#undef EXPECTED_CL_ARGS_NUM
#undef HASH_FUNCS_NUM
#undef HASH_FUNCS_NAMES

/*------------------------------------------------ Read File ---------------------------------------------------------*/

int readFile(char *filename, char** fileContentsContainer) {
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) return 4; // Filed to open file

    if (fseek(file, 0, SEEK_END)) {
        if (fclose(file) != 0) return 5; // file reading failed
        return 5; // file reading failed
    }
    const long fileSize = ftell(file);
    if (fileSize == -1) {
        if (fclose(file) != 0) return 5; // file reading failed
        return 5; // file reading failed
    }
    if (fseek(file, 0, SEEK_SET)) {
        if (fclose(file) != 0) return 5; // file reading failed
        return 5; // file reading failed
    }

    *fileContentsContainer = (char*) malloc((fileSize + 1) * sizeof(char));
    if (*fileContentsContainer == NULL) {
        if (fclose(file) != 0) return 5; // file reading failed
        return 6;  // malloc failed
    }
    (*fileContentsContainer)[fileSize] = '\0';
    if (fread(*fileContentsContainer, fileSize, 1, file) != 1) {
        free(*fileContentsContainer);
        if (fclose(file) != 0) return 5; // file reading failed
        return 5; // file reading failed
    }

    if (fclose(file) != 0) {
        free(*fileContentsContainer);
        return 5; // file reading failed
    }

    return 0;
}

/*---------------------------------------------- Fill Multiset -------------------------------------------------------*/

void fillHashTable(Multiset *hashTable, char *contents) {
    char *word = strtok(contents, " ");
    while (word != NULL) {
        hashTable->addItem(hashTable, word);
        word = strtok(NULL, " ");
    }
}

/*-------------------------------------------------- Main ------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    InputData inputData;
    if (parseClArgs(&inputData, argc, argv)) return EXIT_FAILURE;

    Multiset hashTable;
    if (Multiset_initMultiset(&hashTable, inputData.config) != MULTISET_E_OK) return EXIT_FAILURE;

    char* fileContents = NULL;
    if (readFile(inputData.filename, &fileContents)) return EXIT_FAILURE;

    const clock_t begin = clock();
    fillHashTable(&hashTable, fileContents);
    const clock_t end = clock();
    const double executionTime = (double) (end - begin) / CLOCKS_PER_SEC;

    MultisetStats stats;
    if (hashTable.getStatistics(&hashTable, &stats) != MULTISET_E_OK) {
        free(fileContents);
        return EXIT_FAILURE;
    }
    printf("Words number: %ld\n"
           "Unique by hash words number: %ld\n"
           "The most frequently used word: %s\n"
           "Max number of words with the same hash: %ld\n"
           "Execution time: %lf\n",
           stats.itemsCount, stats.uniqueItemsCount, stats.maxCountWord, stats.maxCount, executionTime);

    free(fileContents);
    if (Multiset_eraseMultiset(&hashTable) != MULTISET_E_OK) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
