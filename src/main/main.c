#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "multiset/multiset.h"

typedef struct {
    char *filename;
    MultisetConfig config;
} InputData;




/*-------------------------------------------------------- Vector ----------------------------------------------------*/

typedef struct {
    char *buf;
    size_t cap;
    size_t size;
} Vector;

Vector *init_vector(size_t cap){
    Vector *vector = malloc(sizeof(Vector));
    if (vector == NULL) return NULL;
    vector->cap = cap;
    vector->buf = malloc(cap); //проверка cap
    vector->size = 0;
    return vector;
}

void push_vector(const char *s, size_t word_len, Vector vector) {
    while (vector.size + word_len + 1 > vector.cap) vector.cap *= 2;
    vector.buf = realloc(vector.buf, vector.cap);
    for (size_t i = 0; i < word_len; i++) {
        vector.buf[vector.size++] = s[i];
    }
    vector.buf[vector.size++] = '\0';
}

void erase_vector(Vector *vector){
    free(vector->buf);
    vector->buf = NULL;
}

/*-------------------------------------------------- Hash Functions --------------------------------------------------*/

static const char *const HASH_FUNC_NAMES[] = {
        "md5",
        "polynomial",
        "sha-1",
};

#define HASH_FUNC_NUM 3

/*------------------------------------------- Parse Command Line Arguments -------------------------------------------*/

#define KEY_LEN 7

static const char *const KEY_SIZE = "--size=";
static const char *const KEY_HASH = "--hash=";
static const char *const KEY_HELP = "--help";

int parse_arguments(InputData *inputData, size_t arguments_number, char **arguments) {
    *inputData = (InputData) {0};

    if (arguments_number == 3) {

        if (arguments[1] == NULL) return 1; // Wrong filename extension
        inputData->filename = arguments[0];

        if (strncmp(arguments[1], KEY_SIZE, KEY_LEN) != 0) return 2; // Wrong size key (argv[2])
        inputData->config.size = (size_t) strtoll(arguments[1] + KEY_LEN, NULL, 10);
        if (inputData->config.size == 0) return 7; // size != 0

        if (strncmp(arguments[2], KEY_HASH, KEY_LEN) != 0) return 3; // Wrong hash key (argv[3])
        char *hashName = arguments[2] + KEY_LEN;
        int isNameCorrect = 0;
        for (size_t i = 0; i < HASH_FUNC_NUM; i++) {
            if (strcmp(hashName, HASH_FUNC_NAMES[i]) == 0) {
                inputData->config.hashFuncLabel = i;
                isNameCorrect = 1;
                break;
            }
        }
        if (!isNameCorrect) return 4; // Wrong name hash

    } else if (arguments_number == 1 && strcmp(arguments[0], KEY_HELP) == 0) {
        printf("documentation\n");
    } else {
        printf("try: --help\n");
        return 5; // Wrong keys number
    }
    return 0;
}

#undef KEY_LEN
#undef HASH_FUNC_NUM

/*------------------------------------------------ Read File ---------------------------------------------------------*/

int readFile(Vector vector, char *filename) {
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) return 6; // File not found

    char c;
    char word[1000];
    size_t count = 0;
    while ((c = (char) fgetc(file)) != EOF) {
        if (isalpha(c)) {
            word[count++] = c;
        } else {
            if (count != 0) {
                push_vector(word, count, vector);
            }
            count = 0;
        }
    }
    if (count) {
        push_vector(word, count, vector);
    }
    fclose(file);
    return 0;
}

/*-------------------------------------------------- Main ------------------------------------------------------------*/

void name_func(Vector *vector, Multiset *table){
    size_t vsize = vector->size;
    size_t i = 0;
    char *word;
    while (i < vsize) {
        word = vector->buf + i;
        while (vector->buf[i] != '\0') i++;
        table->addItem(table, word);
        i++;
    }
}

/*-------------------------------------------------- Main ------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    InputData inputData;
    if (parse_arguments(&inputData, argc - 1, argv + 1)) return EXIT_FAILURE;


    Multiset table, *pTable = &table;
    Multiset_initMultiset(pTable, inputData.config);

    Vector vector = *init_vector(16);
    if (readFile(vector, inputData.filename)) return EXIT_FAILURE;
    clock_t begin = clock();
    name_func(&vector, pTable);
    clock_t end = clock();
    double time = (double) (end - begin) / CLOCKS_PER_SEC;

    MultisetStats stats;
    pTable->getStatistics(pTable, &stats);
    printf("size : %ld\nunique : %ld\nmax : %s\nmax len : %ld\ntime : %lf\n", stats.itemsCount, stats.uniqueItemsCount,
           stats.maxCountWord, stats.maxCount, time);
    Multiset_eraseMultiset(pTable);


    return 0;
}
