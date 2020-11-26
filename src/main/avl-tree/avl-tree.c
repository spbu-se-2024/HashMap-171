#include "avl-tree.h"

#include "avl-tree-impl.c"

#include <stdlib.h>


/*--------------------------------- Memory Management Interface on the Stack and Heap --------------------------------*/

AvlTreeErrCode AvlTree_initAvlTree(AvlTree *avlTree, AvlTreeItemCompF compF, AvlTreeItemFreeF freeF) {
    if (avlTree == NULL) return AVL_TREE_E_NULL_ARG;
    if (compF == NULL) return AVL_TREE_E_NULL_ARG;


    *avlTree = (AvlTree) {0};

    avlTree->_compF = compF;
    avlTree->_freeF = freeF;

    // TODO : Implement interface
    avlTree->find = NULL;
    avlTree->next = NULL;
    avlTree->insert = NULL;
    avlTree->delete = NULL;

    avlTree->traverse = NULL;
    avlTree->clear = NULL;

    return AVL_TREE_E_OK;
}


// TODO : Implement a non-recursive way to free the nodes
static void AvlTree_recursiveNodeFree(AvlTreeNode *node) {
    if (node->left != NULL) AvlTree_recursiveNodeFree(node->left);
    if (node->right != NULL) AvlTree_recursiveNodeFree(node->right);
    free(node);
}

AvlTreeErrCode AvlTree_eraseAvlTree(AvlTree *avlTree) {
    if (avlTree == NULL) return AVL_TREE_E_NULL_ARG;


    if (avlTree->_tree != NULL) AvlTree_recursiveNodeFree(avlTree->_tree);

    *avlTree = (AvlTree) {0};

    return AVL_TREE_E_OK;
}


/*-------------------------------------- Memory Management Interface on the Heap -------------------------------------*/

AvlTreeErrCode AvlTree_allocAvlTree(AvlTree **pAvlTree, AvlTreeItemCompF compF, AvlTreeItemFreeF freeF) {
    if (pAvlTree == NULL) return AVL_TREE_E_NULL_ARG;
    if (compF == NULL) return AVL_TREE_E_NULL_ARG;


    *pAvlTree = malloc(sizeof **pAvlTree);
    if (*pAvlTree == NULL) return AVL_TREE_E_MEM_ALLOC;

    return AvlTree_initAvlTree(*pAvlTree, compF, freeF);
}

AvlTreeErrCode AvlTree_freeAvlTree(AvlTree **pAvlTree) {
    if (pAvlTree == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeErrCode errCode;

    if ((errCode = AvlTree_eraseAvlTree(*pAvlTree))) return errCode;

    *pAvlTree = NULL;

    return AVL_TREE_E_OK;
}
