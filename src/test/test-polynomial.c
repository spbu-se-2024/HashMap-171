#include "../../cutest-1.5/CuTest.h"

#include "../main/hash-functions/hash-function.h"

#include <stdint.h>
#include <string.h>

#ifndef HASH_ELEMS_USED
#define HASH_ELEMS_USED 2
#endif
#ifndef MULT_PRIME
#define MULT_PRIME 257
#endif


/*------------------------------------------ Polynomial - Int - Calculate --------------------------------------------*/

uint64_t ConvertHash(const uint32_t *hash) {
    uint64_t ConvertedHash = 0;
    for (unsigned int i = 0; i < HASH_ELEMS_USED; i++) {
        ConvertedHash = (ConvertedHash << 32u) | hash[i];
    }
    return ConvertedHash;
}

void CheckHash(const char *message, size_t size, uint64_t ExpectedValue, CuTest *tc) {
    uint32_t hash[HASH_ELEMS_USED + 2] = {0};
    polynomial(message, size, &(hash[1]));
    CuAssertIntEquals(tc, 0, hash[0]);
    CuAssertIntEquals(tc, ExpectedValue, ConvertHash(&(hash[1])));
    CuAssertIntEquals(tc, 0, hash[HASH_ELEMS_USED + 1]);
}

void TestPolynomial_EmptyStr_ReturnZero(CuTest *tc) {
    char *message = "";
    size_t size = 0;
    CheckHash(message, size, 0, tc);
}

void TestPolynomial_ShortStr_ReturnHash(CuTest *tc) {
    char *message = "abc";
    size_t size = strlen(message);
    uint64_t ExpectedValue = 0;
    for (unsigned int i = 0; i < size; i++) {
        ExpectedValue = ExpectedValue * MULT_PRIME + message[i];
    }
    CheckHash(message, size, ExpectedValue, tc);
}

void TestPolynomial_LongStr_ReturnHash(CuTest *tc) {
    char *message = "zzzzzzzzzzzzzz";
    size_t size = strlen(message);
    uint64_t ExpectedValue = 0;
    for (unsigned int i = 0; i < size; i++) {
        ExpectedValue = ExpectedValue * MULT_PRIME + message[i];
    }
    CheckHash(message, size, ExpectedValue, tc);
}


/*-------------------------------------------------- Section Name 2 --------------------------------------------------*/


/*-------------------------------------------------- Section Name 3 --------------------------------------------------*/


/*------------------------------------------------------- MAIN -------------------------------------------------------*/

CuSuite *PolynomialGetSuite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestPolynomial_EmptyStr_ReturnZero);
    SUITE_ADD_TEST(suite, TestPolynomial_ShortStr_ReturnHash);
    SUITE_ADD_TEST(suite, TestPolynomial_LongStr_ReturnHash);

    return suite;
}
