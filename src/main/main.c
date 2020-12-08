#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "hash-functions/hash-function.h"

#define KEY_LEN 7
#define SUPPORTED_FILE_FORMAT ".txt"

static const char *const KEY_SIZE = "--size=";
static const char *const KEY_HASH = "--hash=";
static const char *const KEY_HELP = "--help";

typedef HashFuncErrCode (*hash_func_t)(const char *, size_t, uint32_t *);

typedef struct {
    char *filename;
    size_t size;
    hash_func_t hashFunc;
} inputData_t;

/*-------------------------------------------------- Hash Functions --------------------------------------------------*/

static const char *const HASH_FUNC_NAMES[] = {
        "polynomial",
        "sha-1",
        "md5",
};

static const hash_func_t HASH_FUNCTIONS[] = {
        (hash_func_t const) HashFunc_calcPolynomialHash,
        HashFunc_calcSha1Hash,
        HashFunc_calcMd5Hash,
};

static const size_t HASH_NUMBER = sizeof HASH_FUNC_NAMES / sizeof *HASH_FUNCTIONS;

/*------------------------------------------- Parse Command Line Arguments -------------------------------------------*/

int is_str_ending(const char* const str, const char* const ending) {
    const unsigned int str_len = strlen(str), ending_len = strlen(ending);
    if (str_len >= ending_len && strcmp(str + str_len - ending_len, ending) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int parse_arguments(inputData_t *inputData, size_t arguments_number, char **arguments){
    *inputData = (inputData_t) {0};

    if (arguments_number == 3){

        if (!is_str_ending(arguments[0], SUPPORTED_FILE_FORMAT)) return 1; // Wrong filename extension
        inputData->filename = arguments[0];

        if (strncmp(arguments[1], KEY_SIZE, KEY_LEN) != 0) return 2; // Wrong size key (argv[2])
        inputData->size = (size_t) strtoll(arguments[1] + KEY_LEN, NULL, 10);

        if (strncmp(arguments[2], KEY_HASH, KEY_LEN) != 0) return 3; // Wrong hash key (argv[3])
        char *hashName = arguments[2] + KEY_LEN;
        for (size_t i = 0; i < HASH_NUMBER; i++){
            if (strcmp(hashName, HASH_FUNC_NAMES[i]) == 0){
                inputData->hashFunc = HASH_FUNCTIONS[i];
                break;
            }
        }
        if (inputData->hashFunc == NULL) return 4; // Wrong name hash
    }else if (arguments_number == 1 && strcmp(arguments[0], KEY_HELP) == 0){
        printf("documentation\n");
    }else{
        printf("try: --help\n");
        return 5; // Wrong keys number
    }
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    inputData_t inputData;
    parse_arguments(&inputData, argc - 1, argv + 1);
    return 0;
}



/*
 * clock_t begin = clock();
        clock_t end = clock();
        double time = (double) (end - begin) / CLOCKS_PER_SEC;
 */
