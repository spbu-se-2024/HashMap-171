#include "avl-tree.h"

#include "avl-tree-impl.c"


/*--------------------------------- Memory Management Interface on the Stack and Heap --------------------------------*/

AvlTreeErrCode AvlTree_initAvlTree(AvlTree *avlTree, AvlTreeItemCompF compF, AvlTreeItemFreeF freeF) {
    if (avlTree == NULL) return AVL_TREE_E_NULL_ARG;
    if (compF == NULL) return AVL_TREE_E_NULL_ARG;


    *avlTree = (AvlTree) {0};


    avlTree->_compF = compF;
    avlTree->_freeF = freeF;

    // Add implementation
    avlTree->findItem = AvlTree_findItem;
    avlTree->findClosestItem = AvlTree_findClosestItem;
    avlTree->prevNode = AvlTree_prevNode;
    avlTree->nextNode = AvlTree_nextNode;
    avlTree->addItem = AvlTree_addItem;
    avlTree->addItemTimes = AvlTree_addItemTimes;
    avlTree->removeItem = AvlTree_removeItem;
    avlTree->removeItemWithCopies = AvlTree_removeItemWithCopies;

    avlTree->traverse = AvlTree_traverse;
    avlTree->clear = AvlTree_clear;

    return AVL_TREE_E_OK;
}

AvlTreeErrCode AvlTree_eraseAvlTree(AvlTree *avlTree) {
    if (avlTree == NULL) return AVL_TREE_E_NULL_ARG;


    avlTree->clear(avlTree);

    *avlTree = (AvlTree) {0};

    return AVL_TREE_E_OK;
}


/*-------------------------------------- Memory Management Interface on the Heap -------------------------------------*/

AvlTreeErrCode AvlTree_allocAvlTree(AvlTree **pAvlTree, AvlTreeItemCompF compF, AvlTreeItemFreeF freeF) {
    if (pAvlTree == NULL) return AVL_TREE_E_NULL_ARG;
    if (compF == NULL) return AVL_TREE_E_NULL_ARG;


    *pAvlTree = malloc(sizeof **pAvlTree);
    if (*pAvlTree == NULL) {
        return AVL_TREE_E_MEM_ALLOC;
    }

    return AvlTree_initAvlTree(*pAvlTree, compF, freeF);
}

AvlTreeErrCode AvlTree_freeAvlTree(AvlTree **pAvlTree) {
    if (pAvlTree == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTree_eraseAvlTree(*pAvlTree);

    free(*pAvlTree);
    *pAvlTree = NULL;

    return AVL_TREE_E_OK;
}
