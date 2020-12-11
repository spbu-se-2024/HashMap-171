#include "../../cutest-1.5/CuTest.h"

#include "../main/multiset/multiset.h"

#include <stdlib.h>
#include <string.h>

// Config for multiset, which is applied in the tests
#define MULTISET_CONFIG {MULTISET_HASH_FUNC_LABEL_MD5, 10}


/*------------------------------------------------ Multiset - Count --------------------------------------------------*/

static size_t countTreesItems(Multiset *multiset) {
    size_t itemsCounter = 0;
    for (size_t i = 0; i < multiset->_config.size; i++) {
        itemsCounter += multiset->_data[i].itemsCount;
    }
    return itemsCounter;
}

static size_t countTreesUniqueItems(Multiset *multiset) {
    size_t uniqueItemsCounter = 0;
    for (size_t i = 0; i < multiset->_config.size; i++) {
        uniqueItemsCounter += multiset->_data[i].uniqueItemsCount;
    }
    return uniqueItemsCounter;
}


static void TestMultiset_AddCount_EqualsAvlTreeCount(CuTest *tc) {
    Multiset multiset;
    MultisetConfig config = MULTISET_CONFIG;
    Multiset_initMultiset(&multiset, config);

    const size_t strsNum = 4, maxCStrLen = 4;
    char *strs[strsNum];
    for (size_t i = 0; i < strsNum; i++) {
        strs[i] = malloc(maxCStrLen * sizeof(char));
    }
    strcpy(strs[0], "abc");
    strcpy(strs[1], "a");
    strcpy(strs[2], " ");
    strcpy(strs[3], "0");

    CuAssertIntEquals(tc, 0, multiset.itemsCount);
    CuAssertIntEquals(tc, 0, countTreesItems(&multiset));
    for (size_t i = 0; i < strsNum; i++) {
        multiset.addItem(&multiset, strs[i]);
        CuAssertTrue(tc, multiset.itemsCount == i + 1 && multiset.itemsCount == countTreesItems(&multiset));
    }

    CuAssertIntEquals(tc, MULTISET_E_OK, Multiset_eraseMultiset(&multiset));
}

static void TestMultiset_AddCountUnique_EqualsAvlTreeCountUnique(CuTest *tc) {
    Multiset multiset;
    MultisetConfig config = MULTISET_CONFIG;
    Multiset_initMultiset(&multiset, config);

    const size_t strsNum = 5, maxCStrLen = 7;
    char *strs[strsNum];
    for (size_t i = 0; i < strsNum; i++) {
        strs[i] = malloc(maxCStrLen * sizeof(char));
    }
    strcpy(strs[0], "-");
    strcpy(strs[1], "cdeeeG");
    strcpy(strs[2], " ");
    strcpy(strs[3], "123");
    strcpy(strs[4], "cdeeeG");

    CuAssertIntEquals(tc, 0, multiset.uniqueItemsCount);
    CuAssertIntEquals(tc, 0, countTreesUniqueItems(&multiset));
    for (size_t i = 0; i < strsNum - 1; i++) {
        multiset.addItem(&multiset, strs[i]);
        CuAssertTrue(tc, multiset.uniqueItemsCount == i + 1 &&
                         multiset.uniqueItemsCount == countTreesUniqueItems(&multiset));
    }
    multiset.addItem(&multiset, strs[strsNum - 1]);
    CuAssertTrue(tc, multiset.uniqueItemsCount == strsNum - 1 &&
                     multiset.uniqueItemsCount == countTreesUniqueItems(&multiset));

    CuAssertIntEquals(tc, MULTISET_E_OK, Multiset_eraseMultiset(&multiset));
}

static void TestMultiset_AddAllCharsCountMixed_EqualsAvlTreeCountMixed(CuTest *tc) {
    Multiset multiset;
    MultisetConfig config = MULTISET_CONFIG;
    Multiset_initMultiset(&multiset, config);

    const size_t strsNum = 256, maxCStrLen = 2;
    char *strs[strsNum];
    for (size_t i = 0; i < strsNum; i++) {
        strs[i] = malloc(maxCStrLen * sizeof(char));
        strs[i][0] = (char) i;
        strs[i][1] = '\0';
    }

    CuAssertIntEquals(tc, 0, multiset.uniqueItemsCount);
    CuAssertIntEquals(tc, 0, countTreesUniqueItems(&multiset));
    for (size_t i = 0; i < strsNum; i++) {
        multiset.addItem(&multiset, strs[i]);
        CuAssertTrue(tc, multiset.itemsCount == i + 1 && multiset.itemsCount == countTreesItems(&multiset));
        CuAssertTrue(tc, multiset.uniqueItemsCount == i + 1 &&
                         multiset.uniqueItemsCount == countTreesUniqueItems(&multiset));
    }

    CuAssertIntEquals(tc, MULTISET_E_OK, Multiset_eraseMultiset(&multiset));
}


/*------------------------------------------------------- MAIN -------------------------------------------------------*/

CuSuite *MultisetGetSuite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestMultiset_AddCount_EqualsAvlTreeCount);
    SUITE_ADD_TEST(suite, TestMultiset_AddCountUnique_EqualsAvlTreeCountUnique);
    SUITE_ADD_TEST(suite, TestMultiset_AddAllCharsCountMixed_EqualsAvlTreeCountMixed);

    return suite;
}

#undef MULTISET_CONFIG
