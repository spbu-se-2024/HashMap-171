#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "multiset/multiset.h"


#define printErr(errMsg) fprintf(stderr, errMsg)

typedef struct {
    char *filename;
    MultisetConfig config;
} InputData;


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
    bool isHashFuncGiven = 0;
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
#undef HASH_FUNCS_NUM
#undef HASH_FUNCS_NAMES


/*------------------------------------------------ Read File ---------------------------------------------------------*/

#define closeFileAfterErr(file) if (fclose(file) != 0) printErr("Also failed to close the file\n")

unsigned int readFile(char *filename, char **fileContentsContainer) {
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) {
        printErr("Failed to open given file\n");
        return 1;
    }

    if (fseek(file, 0, SEEK_END)) {
        printErr("Failed to read given file (fseek failed)\n");
        closeFileAfterErr(file);
        return 1;
    }
    const long fileSize = ftell(file);
    if (fileSize == -1) {
        printErr("Failed to read given file (ftell failed)\n");
        closeFileAfterErr(file);
        return 1;
    }
    if (fseek(file, 0, SEEK_SET)) {
        printErr("Failed to read given file (fseek failed)\n");
        closeFileAfterErr(file);
        return 1;
    }

    *fileContentsContainer = malloc(fileSize + 1);
    if (*fileContentsContainer == NULL) {
        printErr("Failed to allocate memory for contents of the given file\n");
        closeFileAfterErr(file);
        return 1;
    }
    (*fileContentsContainer)[fileSize] = '\0';
    if (fread(*fileContentsContainer, fileSize, 1, file) != 1) {
        printErr("Failed to read given file (fread failed)\n");
        free(*fileContentsContainer);
        closeFileAfterErr(file);
        return 1;
    }

    if (fclose(file) != 0) {
        printErr("Failed to close given file after reading\n");
        free(*fileContentsContainer);
        return 1;
    }

    return 0;
}

#undef closeFileAfterErr


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

    char *fileContents = NULL;
    if (readFile(inputData.filename, &fileContents)) {
        Multiset_eraseMultiset(&hashTable);
        return EXIT_FAILURE;
    }

    const clock_t begin = clock();
    fillHashTable(&hashTable, fileContents);
    const clock_t end = clock();
    const double executionTime = (double) (end - begin) / CLOCKS_PER_SEC;

    MultisetStats stats;
    if (hashTable.getStatistics(&hashTable, &stats) != MULTISET_E_OK) {
        Multiset_eraseMultiset(&hashTable);
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

#undef printErr
