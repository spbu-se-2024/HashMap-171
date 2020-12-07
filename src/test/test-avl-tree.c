#include "../../cutest-1.5/CuTest.h"

#include "../main/avl-tree/avl-tree.h"

#include <string.h>


/*------------------------------------------------- AvlTree - ErrCodes -----------------------------------------------*/

static int cmpInt(void *a, void *b);

static void dummyFunc() {}


static void TestAvlTree_NullAvlTreePtr_ReturnNullArgErr(CuTest *tc) {
    CuAssertIntEquals_Msg(tc, "initAvlTree failed", AVL_TREE_E_NULL_ARG, AvlTree_initAvlTree(NULL, (AvlTreeItemCompF) dummyFunc, dummyFunc));
    CuAssertIntEquals_Msg(tc, "eraseAvlTree failed", AVL_TREE_E_NULL_ARG, AvlTree_eraseAvlTree(NULL));
}

static void TestAvlTree_NullItemCompPtr_ReturnNullArgErr(CuTest *tc) {
    AvlTree dummyAvlTree;
    CuAssertIntEquals_Msg(tc, "initAvlTree failed", AVL_TREE_E_NULL_ARG, AvlTree_initAvlTree(&dummyAvlTree, NULL, dummyFunc));
}

static void TestAvlTree_NullThis_ReturnNullThisArg(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, dummyFunc);
    int dummyInt;
    AvlTreeNode *dummyNodePtr;
    CuAssertIntEquals_Msg(tc, "findItem failed", AVL_TREE_E_NULL_THIS, avlTree.findItem(NULL, &dummyInt, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "findClosestItem failed", AVL_TREE_E_NULL_THIS, avlTree.findClosestItem(NULL, &dummyInt, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "prevNode failed", AVL_TREE_E_NULL_THIS, avlTree.prevNode(NULL, dummyNodePtr, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "nextNode failed", AVL_TREE_E_NULL_THIS, avlTree.nextNode(NULL, dummyNodePtr, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "addItem failed", AVL_TREE_E_NULL_THIS, avlTree.addItem(NULL, &dummyInt, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "addItemTimes failed", AVL_TREE_E_NULL_THIS, avlTree.addItemTimes(NULL, &dummyInt, 1, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "removeItem failed", AVL_TREE_E_NULL_THIS, avlTree.removeItem(NULL, &dummyInt));
    CuAssertIntEquals_Msg(tc, "removeItemWithCopies failed", AVL_TREE_E_NULL_THIS, avlTree.removeItemWithCopies(NULL, &dummyInt));
    CuAssertIntEquals_Msg(tc, "traverse failed", AVL_TREE_E_NULL_THIS, avlTree.traverse(NULL, &dummyInt, dummyFunc));
    CuAssertIntEquals_Msg(tc, "clear failed", AVL_TREE_E_NULL_THIS, avlTree.clear(NULL));
}


/*------------------------------------------- AvlTree - Int - Insert - Find ------------------------------------------*/

static int cmpInt(void *a, void *b) { return *(int *) a - *(int *) b; }


static void TestAvlTree_Int_InsertFind_Exist(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x = 5;
    avlTree.addItem(&avlTree, &x, NULL);

    int y = 5;
    AvlTreeNode *node;
    avlTree.findItem(&avlTree, &y, &node);

    CuAssertPtrNotNull(tc, node);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_InsertFind_NotExist1(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x = 5;
    avlTree.addItem(&avlTree, &x, NULL);

    int y = 4;
    AvlTreeNode *node;
    avlTree.findItem(&avlTree, &y, &node);

    CuAssertPtrEquals(tc, NULL, node);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_InsertFind_NotExist2(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int x = 5;
    avlTree.addItem(&avlTree, &x, NULL);

    int y = 6;
    AvlTreeNode *node;
    avlTree.findItem(&avlTree, &y, &node);

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
    avlTree.addItem(&avlTree, &x1, NULL);
    avlTree.addItem(&avlTree, &x2, NULL);
    avlTree.addItem(&avlTree, &x3, NULL);
    avlTree.addItem(&avlTree, &x4, NULL);

    int y = 5;
    AvlTreeNode *node;
    avlTree.findItem(&avlTree, &y, &node);

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
    avlTree.addItem(&avlTree, &x1, NULL);
    avlTree.addItem(&avlTree, &x2, NULL);
    avlTree.addItem(&avlTree, &x3, NULL);
    avlTree.addItem(&avlTree, &x4, NULL);

    int y = 2;
    AvlTreeNode *node;
    avlTree.findItem(&avlTree, &y, &node);

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
    avlTree.addItem(&avlTree, &x1, NULL);
    avlTree.addItem(&avlTree, &x2, NULL);
    avlTree.addItem(&avlTree, &x3, NULL);
    avlTree.addItem(&avlTree, &x4, NULL);

    int y = 8;
    AvlTreeNode *node;
    avlTree.findItem(&avlTree, &y, &node);

    CuAssertPtrEquals(tc, NULL, node);

    AvlTree_eraseAvlTree(&avlTree);
}


/*------------------------------------------------------- MAIN -------------------------------------------------------*/

CuSuite *AvlTreeGetSuite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAvlTree_NullAvlTreePtr_ReturnNullArgErr);
    SUITE_ADD_TEST(suite, TestAvlTree_NullItemCompPtr_ReturnNullArgErr);
    SUITE_ADD_TEST(suite, TestAvlTree_NullThis_ReturnNullThisArg);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_Exist);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExist1);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExist2);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_ExistWithDuplicates);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExistWithDuplicates1);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExistWithDuplicates2);

    return suite;
}
