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
} inputData_t;

/*-------------------------------------------------- Hash Functions --------------------------------------------------*/

static const char *const HASH_FUNC_NAMES[] = {
        "md5",
        "polynomial",
        "sha-1",
};

#define HASH_FUNC_NUM 3

/*------------------------------------------- Parse Command Line Arguments -------------------------------------------*/

#define KEY_LEN 7
#define SUPPORTED_FILE_FORMAT ".txt"

static const char *const KEY_SIZE = "--size=";
static const char *const KEY_HASH = "--hash=";
static const char *const KEY_HELP = "--help";

int parse_arguments(inputData_t *inputData, size_t arguments_number, char **arguments) {
    *inputData = (inputData_t) {0};

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
#undef SUPPORTED_FILE_FORMAT
#undef HASH_FUNC_NUM

/*------------------------------------------------ Read File ---------------------------------------------------------*/

int readFile(Multiset *pTable, char *filename){
    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) return 6; // File not found

    char c;
    char word[sizeof(size_t)];
    size_t count = 0;
    while ((c = (char)fgetc(file)) != EOF) {
        if (isalpha(c)){
            word[count++] = c;
        }else{
            if (count != 0) {
                word[count++] = '\0';
                char item[count];
                strncpy(item, word, count);
                if (pTable->addItem(pTable, item)) return 7; // func err
                //printf("%s\n", item);
            }
            count = 0;
        }
    }
    if (count){
        word[count++] = '\0';
        char item[count];
        strncpy(item, word, count);
        if (pTable->addItem(pTable, item)) return 7; // func err
    }
    fclose(file);
    return 0;
}

/*-------------------------------------------------- Main ------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    inputData_t inputData;
    //if (parse_arguments(&inputData, argc - 1, argv + 1)) return -1;

    inputData.filename = "text.txt";
    inputData.config.size = 100;
    inputData.config.hashFuncLabel = 0;

    Multiset table, *pTable = &table;
    Multiset_initMultiset(pTable, inputData.config);



    if (readFile(pTable, inputData.filename)) return -1;


    MultisetStats stats;
    pTable->getStatistics(pTable, &stats);
    printf("size : %ld\nunique : %ld\nmax : %s\nmax len : %ld\n", stats.itemsCount, stats.uniqueItemsCount,
           stats.maxCountWord, stats.maxCount);
    Multiset_eraseMultiset(pTable);


    return 0;
}



/*
        clock_t begin = clock();
        clock_t end = clock();
        double time = (double) (end - begin) / CLOCKS_PER_SEC;
 */
