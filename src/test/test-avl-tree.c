#include "../../cutest-1.5/CuTest.h"

#include "../main/avl-tree/avl-tree.h"

#include <string.h>


/*------------------------------------------- AvlTree - Int - Insert - Find ------------------------------------------*/

static int cmpInt(void *a, void *b) { return *(int *) a - *(int *) b; }


static void TestAvlTree_Int_InsertFind_Exist(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x = 5;
    avlTree.insert(&avlTree, &x, NULL);

    int y = 5;
    AvlTreeNode *node;
    avlTree.find(&avlTree, &y, &node);

    CuAssertPtrNotNull(tc, node);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_InsertFind_NotExist1(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x = 5;
    avlTree.insert(&avlTree, &x, NULL);

    int y = 4;
    AvlTreeNode *node;
    avlTree.find(&avlTree, &y, &node);

    CuAssertPtrEquals(tc, NULL, node);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_InsertFind_NotExist2(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x = 5;
    avlTree.insert(&avlTree, &x, NULL);

    int y = 6;
    AvlTreeNode *node;
    avlTree.find(&avlTree, &y, &node);

    CuAssertPtrEquals(tc, NULL, node);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_InsertFind_ExistWithDuplicates(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x1 = 5;
    int x2 = 5;
    int x3 = 5;
    int x4 = 5;
    avlTree.insert(&avlTree, &x1, NULL);
    avlTree.insert(&avlTree, &x2, NULL);
    avlTree.insert(&avlTree, &x3, NULL);
    avlTree.insert(&avlTree, &x4, NULL);

    int y = 5;
    AvlTreeNode *node;
    avlTree.find(&avlTree, &y, &node);

    CuAssertPtrNotNull(tc, node);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_InsertFind_NotExistWithDuplicates1(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x1 = 5;
    int x2 = 5;
    int x3 = 5;
    int x4 = 5;
    avlTree.insert(&avlTree, &x1, NULL);
    avlTree.insert(&avlTree, &x2, NULL);
    avlTree.insert(&avlTree, &x3, NULL);
    avlTree.insert(&avlTree, &x4, NULL);

    int y = 2;
    AvlTreeNode *node;
    avlTree.find(&avlTree, &y, &node);

    CuAssertPtrEquals(tc, NULL, node);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_InsertFind_NotExistWithDuplicates2(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x1 = 5;
    int x2 = 5;
    int x3 = 5;
    int x4 = 5;
    avlTree.insert(&avlTree, &x1, NULL);
    avlTree.insert(&avlTree, &x2, NULL);
    avlTree.insert(&avlTree, &x3, NULL);
    avlTree.insert(&avlTree, &x4, NULL);

    int y = 8;
    AvlTreeNode *node;
    avlTree.find(&avlTree, &y, &node);

    CuAssertPtrEquals(tc, NULL, node);

    AvlTree_eraseAvlTree(&avlTree);
}


/*------------------------------------------------------- MAIN -------------------------------------------------------*/

CuSuite *AvlTreeGetSuite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_Exist);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExist1);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExist2);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_ExistWithDuplicates);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExistWithDuplicates1);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExistWithDuplicates2);

    return suite;
}
