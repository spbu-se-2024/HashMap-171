#ifndef HASHMAP_171_MULTISET_ERR_H
#define HASHMAP_171_MULTISET_ERR_H

#include "multiset-config.h"
#include "../error-handling/error-handling.h"


/*----------------------------------------------- Multiset Error Codes -----------------------------------------------*/

typedef enum {
    MULTISET_E_OK,
    MULTISET_E_MEM_ALLOC,
    MULTISET_E_NULL_THIS,
    MULTISET_E_NULL_ARG,
    MULTISET_E_WRONG_CONFIG,
    MULTISET_E_OTHER,
} MultisetErrCode;

extern const char *const MULTISET_ERR_MESSAGES[];


/*----------------------------------------- Multiset Error Handling Interface ----------------------------------------*/

#define Multiset_printErr(errStream, errCode) \
    ErrorHandling_printErr(                   \
        Multiset, MULTISET, "Multiset"        \
        errStream, errCode                    \
    )

#define Multiset_autoprintErr(errCode)                 \
    do {                                               \
        if (MULTISET_OPT_1_AUTOPRINT_ERR) {            \
            ErrorHandling_printErr(                    \
                Multiset, MULTISET, "Multiset",        \
                MULTISET_OPT_1_ARG_ERR_STREAM, errCode \
            );                                         \
        }                                              \
    } while(0)


#define Multiset_stopRunOnBadErrCode(errCode, codeToRunOnBadErrCode) \
    ErrorHandling_stopRunOnBadErrCode(                               \
        Multiset, MULTISET, "Multiset",                              \
        errCode, codeToRunOnBadErrCode                               \
    )


#define Multiset_printErrAndStopRunIf(isErr, errStream, errCode, codeToRunIfIsErr) \
    ErrorHandling_printErrAndStopRunIf(                                            \
        Multiset, MULTISET, "Multiset"                                             \
        isErr, errStream, errCode, codeToRunIfIsErr                                \
    )

#define Multiset_autoprintErrAndStopRunIf(isErr, errCode, codeToRunIfIsErr)     \
    do {                                                                        \
        if (MULTISET_OPT_1_AUTOPRINT_ERR) {                                     \
            ErrorHandling_printErrAndStopRunIf(                                 \
                Multiset, MULTISET, "Multiset",                                 \
                isErr, MULTISET_OPT_1_ARG_ERR_STREAM, errCode, codeToRunIfIsErr \
            );                                                                  \
        }                                                                       \
    } while(0)


#endif // HASHMAP_171_MULTISET_ERR_H
