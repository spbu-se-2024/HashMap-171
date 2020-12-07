#include "../../cutest-1.5/CuTest.h"

#include "../main/hash-functions/hash-function.h"

#include <stdint.h>
#include <string.h>


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

void checkPolynomialHash(const char *message, size_t size, uint64_t expectedValue, CuTest *tc) {
    uint64_t hash = 0;
    CuAssertIntEquals(tc, HASH_FUNC_E_OK, calculatePolynomialHash(message, size, &hash));
    CuAssertTrue(tc, hash == expectedValue);
}


void TestPolynomial_JunkInHashContainer_CalculateHash(CuTest *tc) {
    char *message = "abc";
    const size_t size = strlen(message);
    uint64_t hash = 123456789;
    CuAssertIntEquals(tc, HASH_FUNC_E_OK, calculatePolynomialHash(message, size, &hash));
    CuAssertTrue(tc, hash == 6432038);
}

void TestPolynomial_EmptyCStr_CalculateZero(CuTest *tc) {
    char *message = "";
    const size_t size = 0;
    checkPolynomialHash(message, size, 0, tc);
}

void TestPolynomial_ShortCStr_CalculateHash(CuTest *tc) {
    char *message = "ABZ";
    const size_t size = strlen(message);
    checkPolynomialHash(message, size, 4310237, tc);
}

void TestPolynomial_LongCStr_CalculateHash(CuTest *tc) {
    char *message = "zzzzzzzzzzzzzz";
    const size_t size = strlen(message);
    checkPolynomialHash(message, size, 9739836776333403308u, tc);
}

void TestPolynomial_NulInTheMiddle_CalculateHash(CuTest *tc) {
    char message[3] = {'a', '\0', 'b'};
    const size_t size = sizeof(message) / sizeof(message[0]);
    checkPolynomialHash(message, size, 6406851, tc);
}

void TestPolynomial_AllCharsUsed_CalculateHash(CuTest *tc) {
    const size_t size = 256;
    char message[size];
    for (size_t i = 0; i < size; i++) {
        message[i] = (char) i;
    }
    checkPolynomialHash(message, size, 2039883115088510848u, tc);
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
