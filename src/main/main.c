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


/*-------------------------------------------------------- Vector ----------------------------------------------------*/

typedef struct {
    const char **buf;
    size_t cap;
    size_t size;
} Vector;

int vInit(Vector *vector, size_t cap);

int vPush(Vector *vector, const char *s);

void vErase(Vector *vector);


int vInit(Vector *vector, size_t cap) {
    vector->buf = malloc(cap * sizeof *vector->buf);
    printErrAndStopRunIf(vector->buf == NULL, "Cannot allocate memory.\n",);
    vector->cap = cap;
    vector->size = 0;

    return 0;
}

int vPush(Vector *vector, const char *s) {
    if (vector->size == vector->cap) vector->cap *= 2;

    void *tempBuf = realloc(vector->buf, vector->cap * sizeof *vector->buf);
    printErrAndStopRunIf(tempBuf == NULL, "Cannot reallocate memory.\n",);
    vector->buf = tempBuf;

    vector->buf[vector->size++] = s;

    return 0;
}

void vErase(Vector *vector) {
    free(vector->buf);
    vector->buf = NULL;
}


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


/*----------------------------------------------------- Read File ----------------------------------------------------*/

#define MAX_WORD_LEN 10000

int readFile(Vector *v, char *filename) {
    FILE *file;
    file = fopen(filename, "r");
    printErrAndStopRunIf(file == NULL, "Cannot open the file.\n",);

    char word[MAX_WORD_LEN];
    size_t count = 0;

    char c;
    while ((c = (char) fgetc(file)) != EOF) {
        printErrAndStopRunIf(count == MAX_WORD_LEN, "Too long word in the text.\n", { fclose(file); });

        if (isalpha(c)) {
            word[count++] = c;
        } else if (count != 0) {
            word[count++] = '\0';

            char *item = malloc(count);
            printErrAndStopRunIf(item == NULL, "Cannot allocate memory.\n", { fclose(file); });

            strncpy(item, word, count);

            stopRunIf(vPush(v, item), { fclose(file); });

            count = 0;
        }
    }

    if (count != 0) {
        word[count++] = '\0';

        char *item = malloc(count);
        printErrAndStopRunIf(item == NULL, "Cannot allocate memory.\n", { fclose(file); });

        strncpy(item, word, count);

        stopRunIf(vPush(v, item), {
            fclose(file);
        });
    }

    printErrAndStopRunIf(fclose(file) == EOF, "Cannot close the file.\n",);

    return 0;
}


/*------------------------------------------------------ Run App -----------------------------------------------------*/

int runApp(Multiset *multiset, Vector *v) {
    size_t vSize = v->size;
    for (size_t i = 0; i < vSize; ++i) {
        stopRunIf(multiset->addItem(multiset, v->buf[i]),);
    }

    return 0;
}


/*------------------------------------------------------- Main -------------------------------------------------------*/

int main(int argc, char *argv[]) {
    // Read CL Args
    InputData inputData;
    stopRunIf(parseClArgs(&inputData, argc - 1, argv + 1),);


    // Init Structures
    Vector v;
    stopRunIf(vInit(&v, 16), { vErase(&v); });

    Multiset set, *pSet = &set;
    stopRunIf(Multiset_initMultiset(&set, inputData.config), {
        vErase(&v);
        Multiset_eraseMultiset(pSet);
    });


    // Read File & Run App
    stopRunIf(readFile(&v, inputData.filename), {
        vErase(&v);
        Multiset_eraseMultiset(pSet);
    });

    clock_t begin = clock();
    stopRunIf(runApp(pSet, &v), {
        vErase(&v);
        Multiset_eraseMultiset(pSet);
    });
    clock_t end = clock();
    double time = (double) (end - begin) / CLOCKS_PER_SEC;

    vErase(&v);


    // Print stats
    MultisetStats stats;
    stopRunIf(pSet->getStatistics(pSet, &stats), {
        Multiset_eraseMultiset(pSet);
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

    Multiset_eraseMultiset(pSet);


    return 0;
}
