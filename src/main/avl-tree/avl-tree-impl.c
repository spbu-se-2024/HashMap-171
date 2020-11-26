#include "avl-tree.h"

#include <stdbool.h>
#include <stdlib.h>


/*------------------------------------------------------- Utils ------------------------------------------------------*/

static AvlTreeErrCode AvlTree_findMin(AvlTree *this, AvlTreeNode **pMinItemNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (pMinItemNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *node = this->_tree;

    if (node != NULL) while (node->left != NULL) node = node->left;
    *pMinItemNode = node;

    return AVL_TREE_E_OK;
}


/*------------------------------------------------- Find Item's Node -------------------------------------------------*/

static AvlTreeErrCode AvlTree_find(AvlTree *this, void *item, AvlTreeNode **pItemNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;
    if (pItemNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *node = this->_tree;
    while (true) {
        int comp = this->_compF(item, node->item);

        if (comp < 0) {
            if (node->left != NULL) node = node->left; else break;
        } else if (comp == 0) {
            break;
        } else {
            if (node->right != NULL) node = node->right; else break;
        }
    }
    *pItemNode = node;

    return AVL_TREE_E_OK;
}


/*-------------------------------------------------- Clear AVL Tree --------------------------------------------------*/

// TODO : Implement a non-recursive way to free the nodes
static void AvlTree_recursiveNodeFree(AvlTreeNode *node, AvlTreeItemFreeF freeF) {
    if (node->left != NULL) AvlTree_recursiveNodeFree(node->left, freeF);
    if (node->right != NULL) AvlTree_recursiveNodeFree(node->right, freeF);

    if (freeF != NULL) freeF(node->item);
    free(node);
}

static AvlTreeErrCode AvlTree_clear(AvlTree *this) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;

    if (this->_tree != NULL) {
        AvlTree_recursiveNodeFree(this->_tree, this->_freeF);
        this->_tree = NULL;
    }

    return AVL_TREE_E_OK;
}
