#ifndef HASHMAP_171_ERROR_HANDLING_H
#define HASHMAP_171_ERROR_HANDLING_H

#include <stdbool.h>
#include <stdio.h>


/*--------------------------------------------- Error Handling Interface ---------------------------------------------*/

#define ErrorHandling_printErr(ModuleName, MODULE_NAME, Module_Name, errStream, errCode) \
    do {                                                                                 \
        ModuleName##ErrCode ModuleName##_printErr_errCode = (errCode);                   \
        FILE *ModuleName##_printErr_errStream = (errStream);                             \
                                                                                         \
        fprintf(                                                                         \
            ModuleName##_printErr_errStream,                                             \
            "Error: " Module_Name ": file - \"%s\": function - \"%s\": line - %d: %s\n", \
            __FILE__, __func__, __LINE__,                                                \
            MODULE_NAME##_ERR_MESSAGES[ModuleName##_printErr_errCode]                    \
        );                                                                               \
    } while(0)


#define ErrorHandling_stopRunOnBadErrCode(ModuleName, MODULE_NAME, Module_Name, errCode, codeToRunOnBadErrCode) \
    do {                                                                                                        \
        ModuleName##ErrCode ModuleName##_stopRunOnBadErrCode_errCode = (errCode);                               \
                                                                                                                \
        if (ModuleName##_stopRunOnBadErrCode_errCode) {                                                         \
            {codeToRunOnBadErrCode}                                                                             \
            return ModuleName##_stopRunOnBadErrCode_errCode;                                                    \
        }                                                                                                       \
    } while(0)


#define ErrorHandling_printErrAndStopRunIf(ModuleName, MODULE_NAME, Module_Name, isErr, errStream, errCode, codeToRunIfIsErr) \
    do {                                                                                                                      \
        bool ModuleName##_printErrAndStopRunIf_isErr = (isErr);                                                               \
        FILE *ModuleName##_printErrAndStopRunIf_errStream = (errStream);                                                      \
        ModuleName##ErrCode ModuleName##_printErrAndStopRunIf_errCode = (errCode);                                            \
        if (ModuleName##_printErrAndStopRunIf_isErr) {                                                                        \
            ErrorHandling_printErr(                                                                                           \
                ModuleName, MODULE_NAME, Module_Name,                                                                         \
                ModuleName##_printErrAndStopRunIf_errStream, ModuleName##_printErrAndStopRunIf_errCode                        \
            );                                                                                                                \
            {codeToRunIfIsErr}                                                                                                \
            return ModuleName##_printErrAndStopRunIf_errCode;                                                                 \
        }                                                                                                                     \
    } while(0)


#endif // HASHMAP_171_ERROR_HANDLING_H
