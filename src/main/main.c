#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "multiset/multiset.h"


typedef struct {
    char *filename;
    MultisetConfig config;
} InputData;


/*-------------------------------------------------- Error Handling --------------------------------------------------*/

#define printErr(errMsg) fprintf(stderr, errMsg)

#define stopRunIf(isErr, codeToRunOnErr) \
    do {                                 \
        bool _isErr = (isErr);           \
        if (_isErr) {                    \
            {codeToRunOnErr}             \
            return 1;                    \
        }                                \
    } while (false)

#define printErrAndStopRunIf(isErr, errMsg, codeToRunOnErr) \
    do {                                                    \
        bool _isErr = (isErr);                              \
        if (_isErr) {                                       \
            printErr(errMsg);                               \
            {codeToRunOnErr}                                \
            return 1;                                       \
        }                                                   \
    } while (false)


/*------------------------------------------- Parse Command Line Arguments -------------------------------------------*/

#define EXPECTED_CL_ARGS_NUM 3

static const char *HASH_FUNC_NAMES[3] = {
    "md5",
    "polynomial",
    "sha-1",
};


int parseClArgs(InputData *inputData, size_t argc, char **argv) {
    printErrAndStopRunIf(argc != EXPECTED_CL_ARGS_NUM, "Unexpected number of cl args arguments.\n",);


    inputData->filename = argv[0];

    inputData->config.size = strtoull(argv[1], NULL, 0);
    printErrAndStopRunIf(inputData->config.size == 0, "Invalid hash table size given (must be positive).\n",);

    bool isHashFuncGiven = false;
    for (unsigned int i = MULTISET_HASH_FUNC_LABEL_MD5; i <= MULTISET_HASH_FUNC_LABEL_SHA_1; i++) {
        if (strcmp(argv[2], HASH_FUNC_NAMES[i - MULTISET_HASH_FUNC_LABEL_MD5]) == 0) {
            inputData->config.hashFuncLabel = i;
            isHashFuncGiven = true;
        }
    }
    printErrAndStopRunIf(!isHashFuncGiven,
                         "Unsupported hash function name given (must be md5, polynomial or sha-1).\n",);

    return 0;
}


/*------------------------------------------------------ Run App -----------------------------------------------------*/

#define MAX_WORD_LEN 10000

int runApp(Multiset *pTable, char *filename) {
    FILE *file;
    file = fopen(filename, "r");
    printErrAndStopRunIf(file == NULL, "Cannot open the file.\n",);

    char word[MAX_WORD_LEN];
    size_t count = 0;

    char c;
    while ((c = (char) fgetc(file)) != EOF) {
        printErrAndStopRunIf(count == MAX_WORD_LEN, "Too long word in the text.\n", {
            Multiset_eraseMultiset(pTable);
            fclose(file);
        });

        if (isalpha(c)) {
            word[count++] = c;
        } else if (count != 0) {
            word[count++] = '\0';

            char *item = malloc(count);
            printErrAndStopRunIf(item == NULL, "Cannot allocate memory.\n", {
                Multiset_eraseMultiset(pTable);
                fclose(file);
            });

            strncpy(item, word, count);

            stopRunIf(pTable->addItem(pTable, item), {
                fclose(file);
            });

            count = 0;
        }
    }

    if (count != 0) {
        word[count++] = '\0';

        char *item = malloc(count);
        printErrAndStopRunIf(item == NULL, "Cannot allocate memory.\n", {
            Multiset_eraseMultiset(pTable);
            fclose(file);
        });

        strncpy(item, word, count);

        stopRunIf(pTable->addItem(pTable, item), {
            fclose(file);
        });
    }

    printErrAndStopRunIf(fclose(file) == EOF, "Cannot close the file.\n",);

    return 0;
}


/*------------------------------------------------------- Main -------------------------------------------------------*/

int main(int argc, char *argv[]) {
    InputData inputData;
    stopRunIf(parseClArgs(&inputData, argc - 1, argv + 1),);


    Multiset hashTable, *pTable = &hashTable;
    stopRunIf(Multiset_initMultiset(&hashTable, inputData.config),);


    clock_t begin = clock();
    stopRunIf(runApp(pTable, inputData.filename),);
    clock_t end = clock();

    double time = (double) (end - begin) / CLOCKS_PER_SEC;


    MultisetStats stats;
    stopRunIf(pTable->getStatistics(pTable, &stats), {
        Multiset_eraseMultiset(pTable);
    });

    printf(
        "Words number............................. %ld\n"
        "Unique by hash words number.............. %ld\n"
        "The most frequently used word............ %s\n"
        "Max number of words with the same hash... %ld\n"
        "Size..................................... %ld\n"
        "Hash Function............................ %s\n"
        "Execution time........................... %lf\n",
        stats.itemsCount, stats.uniqueItemsCount,
        stats.maxCountWord, stats.maxCount,
        inputData.config.size, HASH_FUNC_NAMES[inputData.config.hashFuncLabel],
        time
    );


    Multiset_eraseMultiset(pTable);

    return 0;
}
