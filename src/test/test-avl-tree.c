#include "../../cutest-1.5/CuTest.h"

#include "../main/avl-tree/avl-tree.h"

#include <string.h>

static int cmpInt(void *a, void *b);


/*------------------------------------------------- AvlTree - ErrCodes -----------------------------------------------*/

static void dummyFunc() {}


static void TestAvlTree_ErrCodes_NullAvlTreePtr_ReturnNullArgErr(CuTest *tc) {
    CuAssertIntEquals_Msg(tc, "initAvlTree failed", AVL_TREE_E_NULL_ARG, AvlTree_initAvlTree(NULL, (AvlTreeItemCompF) dummyFunc, dummyFunc));
    CuAssertIntEquals_Msg(tc, "eraseAvlTree failed", AVL_TREE_E_NULL_ARG, AvlTree_eraseAvlTree(NULL));
}

static void TestAvlTree_ErrCodes_NullItemCompPtr_ReturnNullArgErr(CuTest *tc) {
    AvlTree dummyAvlTree;
    CuAssertIntEquals_Msg(tc, "initAvlTree failed", AVL_TREE_E_NULL_ARG, AvlTree_initAvlTree(&dummyAvlTree, NULL, dummyFunc));
}

static void TestAvlTree_ErrCodes_NullThis_ReturnNullThisErr(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, dummyFunc);
    int dummyInt = 1;
    AvlTreeNode dummyNode = {0}, *dummyNodePtr = &dummyNode;
    CuAssertIntEquals_Msg(tc, "findItem failed", AVL_TREE_E_NULL_THIS, avlTree.findItem(NULL, &dummyInt, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "findClosestItem failed", AVL_TREE_E_NULL_THIS, avlTree.findClosestItem(NULL, &dummyInt, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "prevNode failed", AVL_TREE_E_NULL_THIS, avlTree.prevNode(NULL, &dummyNode, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "nextNode failed", AVL_TREE_E_NULL_THIS, avlTree.nextNode(NULL, &dummyNode, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "addItem failed", AVL_TREE_E_NULL_THIS, avlTree.addItem(NULL, &dummyInt, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "addItemTimes failed", AVL_TREE_E_NULL_THIS, avlTree.addItemTimes(NULL, &dummyInt, 1, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "removeItem failed", AVL_TREE_E_NULL_THIS, avlTree.removeItem(NULL, &dummyInt));
    CuAssertIntEquals_Msg(tc, "removeItemWithCopies failed", AVL_TREE_E_NULL_THIS, avlTree.removeItemWithCopies(NULL, &dummyInt));
    CuAssertIntEquals_Msg(tc, "traverse failed", AVL_TREE_E_NULL_THIS, avlTree.traverse(NULL, &dummyInt, dummyFunc));
    CuAssertIntEquals_Msg(tc, "clear failed", AVL_TREE_E_NULL_THIS, avlTree.clear(NULL));
}

static void TestAvlTree_ErrCodes_NullArgPtrs_ReturnNullArgErr(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, dummyFunc);
    int dummyInt;
    AvlTreeNode *dummyNodePtr;
    CuAssertIntEquals_Msg(tc, "findItem failed on it's 2nd arg", AVL_TREE_E_NULL_ARG, avlTree.findItem(&avlTree, NULL, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "findItem failed on it's 3rd arg", AVL_TREE_E_NULL_ARG, avlTree.findItem(&avlTree, &dummyInt, NULL));
    CuAssertIntEquals_Msg(tc, "findClosestItem failed on it's 2nd arg", AVL_TREE_E_NULL_ARG, avlTree.findClosestItem(&avlTree, NULL, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "findClosestItem failed on it's 3rd arg", AVL_TREE_E_NULL_ARG, avlTree.findClosestItem(&avlTree, &dummyInt, NULL));
    CuAssertIntEquals_Msg(tc, "prevNode failed on it's 2nd arg", AVL_TREE_E_NULL_ARG, avlTree.prevNode(&avlTree, NULL, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "prevNode failed on it's 3rd arg", AVL_TREE_E_NULL_ARG, avlTree.prevNode(&avlTree, dummyNodePtr, NULL));
    CuAssertIntEquals_Msg(tc, "nextNode failed on it's 2nd arg", AVL_TREE_E_NULL_ARG, avlTree.nextNode(&avlTree, NULL, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "nextNode failed on it's 3rd arg", AVL_TREE_E_NULL_ARG, avlTree.nextNode(&avlTree, dummyNodePtr, NULL));
    CuAssertIntEquals_Msg(tc, "addItem failed on it's 2nd arg", AVL_TREE_E_NULL_ARG, avlTree.addItem(&avlTree, NULL, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "addItemTimes failed on it's 2nd arg", AVL_TREE_E_NULL_ARG, avlTree.addItemTimes(&avlTree, NULL, 1, &dummyNodePtr));
    CuAssertIntEquals_Msg(tc, "removeItem failed on it's 2nd arg", AVL_TREE_E_NULL_ARG, avlTree.removeItem(&avlTree, NULL));
    CuAssertIntEquals_Msg(tc, "removeItemWithCopies failed on it's 2nd arg", AVL_TREE_E_NULL_ARG, avlTree.removeItemWithCopies(&avlTree, NULL));
    CuAssertIntEquals_Msg(tc, "traverse failed on it's 3rd arg", AVL_TREE_E_NULL_ARG, avlTree.traverse(&avlTree, &dummyInt, NULL));
}


/*-------------------------------------------- AvlTree - Int - Add - Check -------------------------------------------*/

static _Bool isCorrectBST(struct avl_tree_node_t *avlTreeNode, AvlTreeItemCompF cmp) {
    return (avlTreeNode->left == NULL ||
    (cmp(avlTreeNode->left->item, avlTreeNode->item) < 0 && isCorrectBST(avlTreeNode->left, cmp))) &&
    (avlTreeNode->right == NULL ||
    (cmp(avlTreeNode->item, avlTreeNode->right->item) < 0 && isCorrectBST(avlTreeNode->right, cmp)));
}


static void TestAvlTree_Int_Add_IsCorrectBST1(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[3] = {1, 2, 3};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_Add_IsCorrectBST2(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[7] = {-1, -2, -3, 0, 3, 2, 1};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddMany_IsCorrectBST(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    const size_t intsNum = 501;
    int ints[intsNum];
    for (int i = 0; i < intsNum; i++) {
        ints[i] = i;
    }

    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddWithDuplicates_IsCorrectBST(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[17] = {1, -123, -1, 2, 2, 1000, 0, 1, 22, -333, 1337, 0, 0, 0, 101, 655, 1};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemove_IsCorrectBST1(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[9] = {100, 50, 0, -239, 1, 99, 49, -3, -100};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    for (size_t i = 0; i < intsNum - 1; i++) {
        avlTree.removeItem(&avlTree, &ints[i]);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItem(&avlTree, &ints[intsNum - 1]);
    CuAssertTrue(tc, avlTree._tree == NULL);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemove_IsCorrectBST2(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[9] = {455, 101, -2, 667, 3, 0, 99, -8, 100};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum / 2; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItem(&avlTree, &ints[0]);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    for (size_t i = intsNum / 2; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItem(&avlTree, &ints[intsNum - 1]);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveWithDuplicates_IsCorrectBST(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[5] = {1, 2, 2, 2, 0};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    avlTree.addItem(&avlTree, &ints[0], NULL);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    for (size_t i = 1; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
        avlTree.removeItem(&avlTree, &ints[i]);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItem(&avlTree, &ints[0]);
    CuAssertTrue(tc, avlTree._tree == NULL);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveUnadded_IsCorrectBST(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[4] = {455, -123, 0, -1}, unaddedInt1 = 234567, unaddedInt2 = 98;
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    avlTree.removeItem(&avlTree, &unaddedInt1);
    CuAssertTrue(tc, avlTree._tree == NULL);
    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
        avlTree.removeItem(&avlTree, &unaddedInt1);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    for (size_t i = 0; i < intsNum - 1; i++) {
        avlTree.removeItem(&avlTree, &ints[i]);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
        avlTree.removeItem(&avlTree, &unaddedInt2);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItem(&avlTree, &ints[intsNum - 1]);
    CuAssertTrue(tc, avlTree._tree == NULL);
    avlTree.removeItem(&avlTree, &unaddedInt1);
    CuAssertTrue(tc, avlTree._tree == NULL);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveWithCopies_IsCorrectBST(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[5] = {0, -2, -2, -2, 1};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    avlTree.addItemTimes(&avlTree, &ints[0], 2, NULL);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    for (size_t i = 1; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItemWithCopies(&avlTree, &ints[0]);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    avlTree.removeItemWithCopies(&avlTree, &ints[1]);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    avlTree.removeItem(&avlTree, &ints[intsNum - 1]);
    CuAssertTrue(tc, avlTree._tree == NULL);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveWithCopiesSingle_IsCorrectBST(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[2] = {222, -345};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItemWithCopies(&avlTree, &ints[0]);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    avlTree.removeItemWithCopies(&avlTree, &ints[1]);
    CuAssertTrue(tc, intsNum == 2 && avlTree._tree == NULL);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveWithCopiesUnadded_IsCorrectBST(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[6] = {267, 999, -5, 10000, 999, -7}, unaddedInt1 = 0, unaddedInt2 = -6;
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    avlTree.removeItemWithCopies(&avlTree, &unaddedInt1);
    CuAssertTrue(tc, avlTree._tree == NULL);
    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
        avlTree.removeItemWithCopies(&avlTree, &unaddedInt1);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    for (size_t i = 0; i < intsNum - 1; i++) {
        avlTree.removeItemWithCopies(&avlTree, &ints[i]);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
        avlTree.removeItem(&avlTree, &unaddedInt2);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItem(&avlTree, &ints[intsNum - 1]);
    CuAssertTrue(tc, avlTree._tree == NULL);
    avlTree.removeItem(&avlTree, &unaddedInt1);
    CuAssertTrue(tc, avlTree._tree == NULL);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveMixed_IsCorrectBST(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[20] = {1, 2, 3, -1000, 0, 0, 1, -3, -3, 2, -3, 400, 202, 0, 111, 1, 3, 101, 6, 70}, unaddedInt = -200;
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    avlTree.removeItem(&avlTree, &unaddedInt);
    CuAssertTrue(tc, avlTree._tree == NULL);
    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
        avlTree.removeItem(&avlTree, &ints[i]);
        CuAssertTrue(tc, avlTree._tree == NULL);
        avlTree.removeItem(&avlTree, &ints[i]);
        CuAssertTrue(tc, avlTree._tree == NULL);
    }
    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.removeItem(&avlTree, &unaddedInt);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    for (size_t i = 0; i < intsNum; i++) {
        avlTree.removeItem(&avlTree, &ints[i]);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    }
    avlTree.addItemTimes(&avlTree, &ints[0], 22, NULL);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    avlTree.removeItemWithCopies(&avlTree, &ints[0]);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));
    avlTree.removeItem(&avlTree, &unaddedInt);
    CuAssertTrue(tc, isCorrectBST(avlTree._tree, avlTree._compF));

    AvlTree_eraseAvlTree(&avlTree);
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


/*-------------------------------------------- AvlTree - Int - Add - Count -------------------------------------------*/

static void TestAvlTree_Int_AddCount_CountCorrectly(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[3] = {1, 2, 3};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertIntEquals(tc, i + 1, avlTree.itemsCount);
    }

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddCountUnique_CountUniqueCorrectly(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[4] = {3, 2, 1, 3};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum - 1; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertIntEquals(tc, i + 1, avlTree.uniqueItemsCount);
    }
    avlTree.addItem(&avlTree, &ints[3], NULL);
    CuAssertIntEquals(tc, intsNum - 1, avlTree.uniqueItemsCount);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddCountMixed_CountMixedCorrectly(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[10] = {3, -1, 0, 1000, 266, 267, -100, 0, 3, 3};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum - 3; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertIntEquals(tc, i + 1, avlTree.itemsCount);
        CuAssertIntEquals(tc, i + 1, avlTree.uniqueItemsCount);
    }
    for (size_t i = intsNum - 3; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertIntEquals(tc, i + 1, avlTree.itemsCount);
        CuAssertIntEquals(tc, intsNum - 3, avlTree.uniqueItemsCount);
    }

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveCountMixedEmpty_CountZero(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int num = 9999;

    CuAssertIntEquals(tc, 0, avlTree.itemsCount);
    CuAssertIntEquals(tc, 0, avlTree.uniqueItemsCount);
    avlTree.addItem(&avlTree, &num, NULL);
    avlTree.removeItem(&avlTree, &num);
    CuAssertIntEquals(tc, 0, avlTree.itemsCount);
    CuAssertIntEquals(tc, 0, avlTree.uniqueItemsCount);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveCountMixed_CountMixedCorrectly(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[19] = {1, 2, 3, 3, 2, 1, -888, 123, 0, 0, -9, 111, 1000, 823, 9, -9, 2, 987324, 12};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < 3; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertIntEquals(tc, i + 1, avlTree.itemsCount);
        CuAssertIntEquals(tc, i + 1, avlTree.uniqueItemsCount);
    }
    for (size_t i = 3; i < 6; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertIntEquals(tc, i + 1, avlTree.itemsCount);
        CuAssertIntEquals(tc, 3, avlTree.uniqueItemsCount);
    }
    for (size_t i = 6; i < intsNum; i++) {
        avlTree.addItem(&avlTree, &ints[i], NULL);
        CuAssertIntEquals(tc, i + 1, avlTree.itemsCount);
        CuAssertTrue(tc, avlTree.uniqueItemsCount < avlTree.itemsCount);
    }
    CuAssertIntEquals(tc, 13, avlTree.uniqueItemsCount);

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddTimesCountMixed_CountMixedCorrectly(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[4] = {1000, 998, 999, -3};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);
    size_t addedIntsNum = 0;

    for (size_t i = 0; i < intsNum; i++) {
        avlTree.addItemTimes(&avlTree, &ints[i], (2 * i + 1) * (i + 5), NULL);
        addedIntsNum += (2 * i + 1) * (i + 5);
        CuAssertIntEquals(tc, addedIntsNum, avlTree.itemsCount);
        CuAssertIntEquals(tc, i + 1, avlTree.uniqueItemsCount);
    }

    AvlTree_eraseAvlTree(&avlTree);
}

static void TestAvlTree_Int_AddRemoveMixedCountMixed_CountMixedCorrectly(CuTest *tc) {
    AvlTree avlTree;
    AvlTree_initAvlTree(&avlTree, cmpInt, NULL);

    int ints[6] = {998, 2, -2, 0, 1, 2};
    const size_t intsNum = sizeof(ints) / sizeof(ints[0]);

    for (size_t i = 0; i < intsNum - 1; i++) {
        avlTree.addItemTimes(&avlTree, &ints[i], 2, NULL);
        CuAssertIntEquals(tc, (i + 1) * 2, avlTree.itemsCount);
        CuAssertIntEquals(tc, i + 1, avlTree.uniqueItemsCount);
    }
    for (size_t i = 0; i < intsNum - 1; i++) {
        avlTree.removeItemWithCopies(&avlTree, &ints[i]);
        CuAssertIntEquals(tc, (intsNum - 2 - i) * 2, avlTree.itemsCount);
        CuAssertIntEquals(tc, intsNum - 2 - i, avlTree.uniqueItemsCount);
    }
    avlTree.addItem(&avlTree, &ints[intsNum - 1], NULL);
    CuAssertIntEquals(tc, 1, avlTree.itemsCount);
    CuAssertIntEquals(tc, 1, avlTree.uniqueItemsCount);
    avlTree.addItemTimes(&avlTree, &ints[intsNum - 1], 999, NULL);
    CuAssertIntEquals(tc, 1000, avlTree.itemsCount);
    CuAssertIntEquals(tc, 1, avlTree.uniqueItemsCount);
    avlTree.removeItem(&avlTree, &ints[intsNum - 1]);
    CuAssertIntEquals(tc, 999, avlTree.itemsCount);
    CuAssertIntEquals(tc, 1, avlTree.uniqueItemsCount);

    AvlTree_eraseAvlTree(&avlTree);
}


/*------------------------------------------------------- MAIN -------------------------------------------------------*/

CuSuite *AvlTreeGetSuite() {
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAvlTree_ErrCodes_NullAvlTreePtr_ReturnNullArgErr);
    SUITE_ADD_TEST(suite, TestAvlTree_ErrCodes_NullItemCompPtr_ReturnNullArgErr);
    SUITE_ADD_TEST(suite, TestAvlTree_ErrCodes_NullThis_ReturnNullThisErr);
    SUITE_ADD_TEST(suite, TestAvlTree_ErrCodes_NullArgPtrs_ReturnNullArgErr);

    SUITE_ADD_TEST(suite, TestAvlTree_Int_Add_IsCorrectBST1);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_Add_IsCorrectBST2);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddMany_IsCorrectBST);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddWithDuplicates_IsCorrectBST);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemove_IsCorrectBST1);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemove_IsCorrectBST2);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveWithDuplicates_IsCorrectBST);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveUnadded_IsCorrectBST);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveWithCopies_IsCorrectBST);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveWithCopiesSingle_IsCorrectBST);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveWithCopiesUnadded_IsCorrectBST);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveMixed_IsCorrectBST);

    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_Exist);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExist1);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExist2);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_ExistWithDuplicates);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExistWithDuplicates1);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_InsertFind_NotExistWithDuplicates2);

    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddCount_CountCorrectly);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddCountUnique_CountUniqueCorrectly);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddCountMixed_CountMixedCorrectly);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveCountMixedEmpty_CountZero);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveCountMixed_CountMixedCorrectly);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddTimesCountMixed_CountMixedCorrectly);
    SUITE_ADD_TEST(suite, TestAvlTree_Int_AddRemoveMixedCountMixed_CountMixedCorrectly);

    return suite;
}
