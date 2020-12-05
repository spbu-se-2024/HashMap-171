#include "../../cutest-1.5/CuTest.h"

#include "../main/hash-functions/hash-function.h"

#include <stdint.h>
#include <string.h>

#ifndef MULT_PRIME
#define MULT_PRIME 257 // A const prime by which chars are multiplied in the polynomial hash function
#endif


/*------------------------------------------- Polynomial - ErrCodes --------------------------------------------------*/


void TestPolynomial_NullMessagePtr_NullArgError(CuTest *tc) {
    size_t size = 1;
    uint64_t hash = 0;
    CuAssertIntEquals(tc, HASH_FUNC_E_NULL_ARG, calculatePolynomialHash(NULL, size, &hash));
}

void TestPolynomial_NullHashPtr_NullArgError(CuTest *tc) {
    char *message = "abc";
    size_t size = strlen(message);
    CuAssertIntEquals(tc, HASH_FUNC_E_NULL_ARG, calculatePolynomialHash(message, size, NULL));
}

/*---------------------------------------- Polynomial - CalculateHash ------------------------------------------------*/

uint64_t calculateExpectedValue(const char *message, size_t size) {
    uint64_t expectedValue = 0;
    for (size_t i = 0; i < size; i++) {
        expectedValue = expectedValue * MULT_PRIME + message[i];
    }
    return expectedValue;
}

void checkHash(const char *message, size_t size, uint64_t expectedValue, CuTest *tc) {
    uint64_t hash = 0;
    CuAssertIntEquals(tc, HASH_FUNC_E_OK, calculatePolynomialHash(message, size, &hash));
    CuAssertIntEquals(tc, expectedValue, hash);
}

void TestPolynomial_JunkInHashContainer_CalculateHash(CuTest *tc) {
    char *message = "abc";
    size_t size = strlen(message);
    uint64_t hash = 123456789;
    CuAssertIntEquals(tc, HASH_FUNC_E_NULL_ARG, calculatePolynomialHash(message, size, &hash));
}

void TestPolynomial_EmptyCStr_CalculateZero(CuTest *tc) {
    char *message = "";
    size_t size = 0;
    checkHash(message, size, 0, tc);
}

void TestPolynomial_ShortCStr_CalculateHash(CuTest *tc) {
    char *message = "abc";
    size_t size = strlen(message);
    uint64_t expectedValue = calculateExpectedValue(message, size);
    checkHash(message, size, expectedValue, tc);
}

void TestPolynomial_LongCStr_CalculateHash(CuTest *tc) {
    char *message = "zzzzzzzzzzzzzz";
    size_t size = strlen(message);
    uint64_t expectedValue = calculateExpectedValue(message, size);
    checkHash(message, size, expectedValue, tc);
}

void TestPolynomial_NulInTheMiddle_CalculateHash(CuTest *tc) {
    char message[3] = {'a', '\0', 'b'};
    size_t size = 3;
    uint64_t expectedValue = calculateExpectedValue(message, size);
    checkHash(message, size, expectedValue, tc);
}

void TestPolynomial_AllCharsUsed_CalculateHash(CuTest *tc) {
    size_t size = 256;
    char message[size];
    for (size_t i = 0; i < size; i++) {
        message[i] = i;
    }
    uint64_t expectedValue = calculateExpectedValue(message, size);
    checkHash(message, size, expectedValue, tc);
}


/*------------------------------------------------------- MAIN -------------------------------------------------------*/

CuSuite *PolynomialGetSuite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestPolynomial_NullMessagePtr_NullArgError);
    SUITE_ADD_TEST(suite, TestPolynomial_NullHashPtr_NullArgError);
    SUITE_ADD_TEST(suite, TestPolynomial_JunkInHashContainer_CalculateHash);
    SUITE_ADD_TEST(suite, TestPolynomial_EmptyCStr_CalculateZero);
    SUITE_ADD_TEST(suite, TestPolynomial_ShortCStr_CalculateHash);
    SUITE_ADD_TEST(suite, TestPolynomial_LongCStr_CalculateHash);
    SUITE_ADD_TEST(suite, TestPolynomial_NulInTheMiddle_CalculateHash);
    SUITE_ADD_TEST(suite, TestPolynomial_AllCharsUsed_CalculateHash);

    return suite;
}

#undef MULT_PRIME
