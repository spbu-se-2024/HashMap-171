#include "../../cutest-1.5/CuTest.h"

#include "../main/hash-functions/hash-function.h"

#include <stdint.h>
#include <string.h>

#define BLOCKS_USED_NUM 4 // Number of blocks used by MD5 in a hash array


/*-------------------------------------------------- MD5 - ErrCodes --------------------------------------------------*/

void TestMd5_NullMessagePtr_NullArgError(CuTest *tc) {
    const size_t size = 1;
    uint32_t hash[BLOCKS_USED_NUM] = {0};
    CuAssertIntEquals(tc, HASH_FUNC_E_NULL_ARG, calculateMd5Hash(NULL, size, hash));
}

void TestMd5_NullHashPtr_NullArgError(CuTest *tc) {
    const char *message = "abc";
    const size_t size = strlen(message);
    CuAssertIntEquals(tc, HASH_FUNC_E_NULL_ARG, calculateMd5Hash(message, size, NULL));
}


/*----------------------------------------------- MD5 - CalculateHash ------------------------------------------------*/

void checkMd5Hash(const char *message, size_t size, const uint32_t *expectedValue, CuTest *tc) {
    uint32_t hash[BLOCKS_USED_NUM] = {0};
    CuAssertIntEquals(tc, HASH_FUNC_E_OK, calculateMd5Hash(message, size, hash));
    CuAssertTrue(tc, memcmp(hash, expectedValue, BLOCKS_USED_NUM) == 0);
}


void TestMd5_JunkInHashContainer_CalculateHash(CuTest *tc) {
    const char *message = "abc";
    const size_t size = strlen(message);
    uint32_t hash[BLOCKS_USED_NUM] = {123, 456, 789, 123};
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0x90015098, 0x3CD24FB0, 0xD6963F7D, 0x28E17F72};
    CuAssertIntEquals(tc, HASH_FUNC_E_OK, calculateMd5Hash(message, size, hash));
    CuAssertTrue(tc, memcmp(hash, expectedValue, BLOCKS_USED_NUM) == 0);
}

void TestMd5_EmptyCStr_CalculateHash(CuTest *tc) {
    char *message = "";
    size_t size = 0;
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0xD41D8CD9, 0x8F00B204, 0xE9800998, 0xECF8427E};
    checkMd5Hash(message, size, expectedValue, tc);
}

void TestMd5_ShortCStr_CalculateHash(CuTest *tc) {
    char *message = "0";
    size_t size = strlen(message);
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0xCFCD2084, 0x95D565EF, 0x66E7DFF9, 0xF98764DA};
    checkMd5Hash(message, size, expectedValue, tc);
}

void TestMd5_LongCStr_CalculateHash(CuTest *tc) {
    char *message = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
    size_t size = strlen(message);
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0x433C3EEE, 0xD47F6134, 0x89500618, 0x6D9F1D21};
    checkMd5Hash(message, size, expectedValue, tc);
}

void TestMd5_NulInTheMiddle_CalculateHash(CuTest *tc) {
    char message[3] = {'a', '\0', 'b'};
    const size_t size = sizeof(message) / sizeof(message[0]);
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0x70350F60, 0x27BCE371, 0x3F6B7647, 0x3084309B};
    checkMd5Hash(message, size, expectedValue, tc);
}

void TestMd5_AllCharsUsed_CalculateHash(CuTest *tc) {
    const size_t size = 256;
    char message[size];
    for (size_t i = 0; i < size; i++) {
        message[i] = (char) i;
    }
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0xE2C865DB, 0x4162BED9, 0x63BFAA9E, 0xF6AC18F0};
    checkMd5Hash(message, size, expectedValue, tc);
}


/*------------------------------------------------------- MAIN -------------------------------------------------------*/

CuSuite *Md5GetSuite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestMd5_NullMessagePtr_NullArgError);
    SUITE_ADD_TEST(suite, TestMd5_NullHashPtr_NullArgError);
    SUITE_ADD_TEST(suite, TestMd5_JunkInHashContainer_CalculateHash);
    SUITE_ADD_TEST(suite, TestMd5_EmptyCStr_CalculateHash);
    SUITE_ADD_TEST(suite, TestMd5_ShortCStr_CalculateHash);
    SUITE_ADD_TEST(suite, TestMd5_LongCStr_CalculateHash);
    SUITE_ADD_TEST(suite, TestMd5_NulInTheMiddle_CalculateHash);
    SUITE_ADD_TEST(suite, TestMd5_AllCharsUsed_CalculateHash);

    return suite;
}
