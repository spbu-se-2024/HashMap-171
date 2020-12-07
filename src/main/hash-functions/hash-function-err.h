#ifndef HASHMAP_171_HASH_FUNCTION_ERR_H
#define HASHMAP_171_HASH_FUNCTION_ERR_H

#include "hash-function-config.h"
#include "../error-handling/error-handling.h"


/*-------------------------------------------- Hash Functions Error Codes --------------------------------------------*/

typedef enum {
    HASH_FUNC_E_OK,
    HASH_FUNC_E_NULL_ARG,
    HASH_FUNC_E_MEM_ALLOC,
    HASH_FUNC_E_OTHER,
} HashFuncErrCode;

extern const char *const HASH_FUNC_ERR_MESSAGES[];


/*-------------------------------------- Hash Functions Error Handling Interface -------------------------------------*/

#define HashFunc_printErr(errStream, errCode) \
    ErrorHandling_printErr(                   \
        HashFunc, HASH_FUNC, "Hash Functions" \
        errStream, errCode                    \
    )

#define HashFunc_autoprintErr(errCode)                  \
    do {                                                \
        if (HASH_FUNC_OPT_1_AUTOPRINT_ERR) {            \
            ErrorHandling_printErr(                     \
                HashFunc, HASH_FUNC, "Hash Functions",  \
                HASH_FUNC_OPT_1_ARG_ERR_STREAM, errCode \
            );                                          \
        }                                               \
    } while(0)


#define HashFunc_stopRunOnBadErrCode(errCode, codeToRunOnBadErrCode) \
    ErrorHandling_stopRunOnBadErrCode(                               \
        HashFunc, HASH_FUNC, "Hash Functions",                       \
        errCode, codeToRunOnBadErrCode                               \
    )


#define HashFunc_printErrAndStopRunIf(isErr, errStream, errCode, codeToRunIfIsErr) \
    ErrorHandling_printErrAndStopRunIf(                                            \
        HashFunc, HASH_FUNC, "Hash Functions"                                      \
        isErr, errStream, errCode, codeToRunIfIsErr                                \
    )

#define HashFunc_autoprintErrAndStopRunIf(isErr, errCode, codeToRunIfIsErr)      \
    do {                                                                         \
        if (HASH_FUNC_OPT_1_AUTOPRINT_ERR) {                                     \
            ErrorHandling_printErrAndStopRunIf(                                  \
                HashFunc, HASH_FUNC, "Hash Functions",                           \
                isErr, HASH_FUNC_OPT_1_ARG_ERR_STREAM, errCode, codeToRunIfIsErr \
            );                                                                   \
        }                                                                        \
    } while(0)


#endif // HASHMAP_171_HASH_FUNCTION_ERR_H
