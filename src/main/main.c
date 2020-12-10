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

#define EXPECTED_ARGS_NUM 3
#define SUPPORTED_FORMAT ".txt"
#define HASH_FUNCS_NAMES {"md5","polynomial","sha-1"}
#define HASH_FUNCS_NUM 3

static _Bool isStrEnding(const char* const str, const char* const ending) {
    const size_t str_len = strlen(str), ending_len = strlen(ending);
    return (str_len >= ending_len && strcmp(str + str_len - ending_len, ending) == 0);
}

int parseClArgs(InputData *inputData, size_t argsNum, char **args) {
    *inputData = (InputData) {0};

    if (argsNum != EXPECTED_ARGS_NUM + 1) {
        return 1; // Unexpected args number
    }
    inputData->filename = args[1];
    if (!isStrEnding(inputData->filename, SUPPORTED_FORMAT)) {
        return 2; // Unsupported file extension
    }
    inputData->config.size = strtoul(args[2], NULL, 10);
    if (inputData->config.size == 0 || !('0' <= args[2][0] && args[2][0] <= '9')) {
        return 3; // Unsupported size
    }
    const char* hashFuncsNames[HASH_FUNCS_NUM] = HASH_FUNCS_NAMES;
    _Bool hashFuncGiven = 0;
    for (unsigned int i = MULTISET_HASH_FUNC_LABEL_MD5; i <= MULTISET_HASH_FUNC_LABEL_SHA_1; i++) {
        if (strcmp(args[3], hashFuncsNames[i - MULTISET_HASH_FUNC_LABEL_MD5]) == 0) {
            inputData->config.hashFuncLabel = i;
            hashFuncGiven = 1;
        }
    }
    if (!hashFuncGiven) {
        return 4; // Unsupported hash function
    }

    return 0;
}

#undef EXPECTED_ARGS_NUM
#undef SUPPORTED_FORMAT
#undef HASH_FUNCS_NAMES
#undef HASH_FUNCS_NUM

/*------------------------------------------------ Read File ---------------------------------------------------------*/

int readFile(char *filename, char** fileContentsContainer) {
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) return 6; // File not found

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    *fileContentsContainer = (char*) malloc((fileSize + 1) * sizeof(char));
    fread(*fileContentsContainer, fileSize, 1, file);
    fclose(file);
    (*fileContentsContainer)[fileSize] = '\0';

    return 0;
}

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
    Multiset_initMultiset(&hashTable, inputData.config);

    char* fileContents = NULL;
    if (readFile(inputData.filename, &fileContents)) return EXIT_FAILURE;

    clock_t begin = clock();
    fillHashTable(&hashTable, fileContents);
    clock_t end = clock();

    double time = (double) (end - begin) / CLOCKS_PER_SEC;

    MultisetStats stats;
    hashTable.getStatistics(&hashTable, &stats);
    printf("size : %ld\n"
           "unique : %ld\n"
           "max : %s\n"
           "max len : %ld\n"
           "time : %lf\n",
           stats.itemsCount, stats.uniqueItemsCount, stats.maxCountWord, stats.maxCount, time);

    Multiset_eraseMultiset(&hashTable);
    free(fileContents);

    return 0;
}
