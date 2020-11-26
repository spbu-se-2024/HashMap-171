#include "avl-tree.h"

#include <stdlib.h>


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
