#include "../../cutest-1.5/CuTest.h"

#include "../main/hash-functions/hash-function.h"

#include <stdint.h>
#include <string.h>

#define BLOCKS_USED_NUM 5 // Number of blocks used by SHA-1 in a hash array


/*------------------------------------------------ SHA-1 - ErrCodes --------------------------------------------------*/

void TestSha1_NullMessagePtr_NullArgError(CuTest *tc) {
    const size_t size = 1;
    uint32_t hash[BLOCKS_USED_NUM] = {0};
    CuAssertIntEquals(tc, HASH_FUNC_E_NULL_ARG, calculateSha1Hash(NULL, size, hash));
}

void TestSha1_NullHashPtr_NullArgError(CuTest *tc) {
    const char *message = "abc";
    const size_t size = strlen(message);
    CuAssertIntEquals(tc, HASH_FUNC_E_NULL_ARG, calculateSha1Hash(message, size, NULL));
}


/*--------------------------------------------- SHA-1 - CalculateHash ------------------------------------------------*/

void checkSha1Hash(const char *message, size_t size, const uint32_t *expectedValue, CuTest *tc) {
    uint32_t hash[BLOCKS_USED_NUM] = {0};
    CuAssertIntEquals(tc, HASH_FUNC_E_OK, calculateSha1Hash(message, size, hash));
    CuAssertTrue(tc, memcmp(hash, expectedValue, BLOCKS_USED_NUM) == 0);
}


void TestSha1_JunkInHashContainer_CalculateHash(CuTest *tc) {
    const char *message = "abc";
    const size_t size = strlen(message);
    uint32_t hash[BLOCKS_USED_NUM] = {123, 456, 789, 123, 999};
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0xA9993E36, 0x4706816A, 0xBA3E2571, 0x7850C26C, 0x9CD0D89D};
    CuAssertIntEquals(tc, HASH_FUNC_E_OK, calculateSha1Hash(message, size, hash));
    CuAssertTrue(tc, memcmp(hash, expectedValue, BLOCKS_USED_NUM) == 0);
}

void TestSha1_EmptyCStr_CalculateHash(CuTest *tc) {
    char *message = "";
    const size_t size = 0;
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0xDA39A3EE, 0x5E6B4B0D, 0x3255BFEF, 0x95601890, 0xAFD80709};
    checkSha1Hash(message, size, expectedValue, tc);
}

void TestSha1_ShortCStr_CalculateHash(CuTest *tc) {
    char *message = "0";
    const size_t size = strlen(message);
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0xB6589FC6, 0xAB0DC82C, 0xF12099D1, 0xC2D40AB9, 0x94E8410C};
    checkSha1Hash(message, size, expectedValue, tc);
}

void TestSha1_LongCStr_CalculateHash(CuTest *tc) {
    char *message = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
    const size_t size = strlen(message);
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0xF5CE5249, 0xF3985567, 0xC7B10DFB, 0xA2464D43, 0x49F5E126};
    checkSha1Hash(message, size, expectedValue, tc);
}

void TestSha1_NulInTheMiddle_CalculateHash(CuTest *tc) {
    char message[3] = {'a', '\0', 'b'};
    const size_t size = sizeof(message) / sizeof(message[0]);
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0x4A3DEC2D, 0x1F824528, 0x0855C42D, 0xB0EE4239, 0xF917FDB8};
    checkSha1Hash(message, size, expectedValue, tc);
}

void TestSha1_AllCharsUsed_CalculateHash(CuTest *tc) {
    const size_t size = 256;
    char message[size];
    for (size_t i = 0; i < size; i++) {
        message[i] = (char) i;
    }
    const uint32_t expectedValue[BLOCKS_USED_NUM] = {0x4916D6BD, 0xB7F78E68, 0x03698CAB, 0x32D1586E, 0xA457DFC8};
    checkSha1Hash(message, size, expectedValue, tc);
}


/*------------------------------------------------------- MAIN -------------------------------------------------------*/

CuSuite *Sha1GetSuite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestSha1_NullMessagePtr_NullArgError);
    SUITE_ADD_TEST(suite, TestSha1_NullHashPtr_NullArgError);
    SUITE_ADD_TEST(suite, TestSha1_JunkInHashContainer_CalculateHash);
    SUITE_ADD_TEST(suite, TestSha1_EmptyCStr_CalculateHash);
    SUITE_ADD_TEST(suite, TestSha1_ShortCStr_CalculateHash);
    SUITE_ADD_TEST(suite, TestSha1_LongCStr_CalculateHash);
    SUITE_ADD_TEST(suite, TestSha1_NulInTheMiddle_CalculateHash);
    SUITE_ADD_TEST(suite, TestSha1_AllCharsUsed_CalculateHash);

    return suite;
}

#undef BLOCKS_USED_NUM
