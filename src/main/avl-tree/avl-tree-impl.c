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

static AvlTreeErrCode AvlTree_findMax(AvlTree *this, AvlTreeNode **pMaxItemNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (pMaxItemNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *node = this->_tree;

    if (node != NULL) while (node->right != NULL) node = node->right;

    *pMaxItemNode = node;

    return AVL_TREE_E_OK;
}


static inline size_t AvlTree_maxSizeT(size_t a, size_t b) { return a < b ? b : a; }

static inline size_t AvlTree_getNodeHeight(AvlTreeNode *node) { return node != NULL ? node->height : 0; }


static void AvlTree_updateHeight(AvlTreeNode *node) {
    if (node == NULL) return;
    node->height = AvlTree_maxSizeT(AvlTree_getNodeHeight(node->left), AvlTree_getNodeHeight(node->right)) + 1;
    AvlTree_updateHeight(node->parent);
}


/*------------------------------------------------- Find Item's Node -------------------------------------------------*/

static AvlTreeErrCode AvlTree_find(AvlTree *this, void *item, AvlTreeNode **pItemNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;
    if (pItemNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *node = this->_tree;

    while (node != NULL) {
        int comp = this->_compF(item, node->item);

        if (comp < 0) node = node->left;
        else if (comp == 0) break;
        else node = node->right;
    }

    *pItemNode = node;

    return AVL_TREE_E_OK;
}

static AvlTreeErrCode AvlTree_findClosest(AvlTree *this, void *item, AvlTreeNode **pItemNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;
    if (pItemNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *node = this->_tree;

    if (node != NULL) {
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
    }

    *pItemNode = node;

    return AVL_TREE_E_OK;
}


/*----------------------------------------------- Previous & Next Node -----------------------------------------------*/

static AvlTreeErrCode AvlTree_prev(AvlTree *this, AvlTreeNode *node, AvlTreeNode **pPrevNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (node == NULL) return AVL_TREE_E_NULL_ARG;
    if (pPrevNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *minItemNode;
    AvlTree_findMin(this, &minItemNode);

    if (this->_tree == NULL || minItemNode == node) {
        node = NULL;
    } else if (node->left != NULL) {
        node = node->left;
        while (node->right != NULL) node = node->right;
    } else {
        while (this->_compF(node->item, node->parent->item) < 0) node = node->parent;
        node = node->parent;
    }

    *pPrevNode = node;

    return AVL_TREE_E_OK;
}

static AvlTreeErrCode AvlTree_next(AvlTree *this, AvlTreeNode *node, AvlTreeNode **pNextNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (node == NULL) return AVL_TREE_E_NULL_ARG;
    if (pNextNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *maxItemNode;
    AvlTree_findMax(this, &maxItemNode);

    if (this->_tree == NULL || maxItemNode == node) {
        node = NULL;
    } else if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL) node = node->left;
    } else {
        while (this->_compF(node->item, node->parent->item) > 0) node = node->parent;
        node = node->parent;
    }

    *pNextNode = node;

    return AVL_TREE_E_OK;
}


/*---------------------------------------------------- Insert Item ---------------------------------------------------*/

static AvlTreeErrCode AvlTree_insert(AvlTree *this, void *item, AvlTreeNode **pNewNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;
    // Possibly optional
    // if (pNewNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeErrCode errCode;

    AvlTreeNode *node;
    if ((errCode = this->findClosest(this, item, &node))) return errCode;

    if (node == NULL) {
        this->_tree = malloc(sizeof(AvlTreeNode));
        if (this->_tree == NULL) return AVL_TREE_E_MEM_ALLOC;

        *this->_tree = (AvlTreeNode) {item, .count = 1, .height = 1};

        node = this->_tree;
    } else {
        int comp = this->_compF(item, node->item);

        if (comp < 0) {
            node->left = malloc(sizeof(AvlTreeNode));
            if (node->left == NULL) return AVL_TREE_E_MEM_ALLOC;

            *node->left = (AvlTreeNode) {item, .count = 1, .height = 1, .parent = node};

            node = node->left;
        } else if (comp == 0) {
            ++node->count;
            this->_freeF(item);
        } else {
            node->right = malloc(sizeof(AvlTreeNode));
            if (node->right == NULL) return AVL_TREE_E_MEM_ALLOC;

            *node->right = (AvlTreeNode) {item, .count = 1, .height = 1, .parent = node};

            node = node->right;
        }
    }

    AvlTree_updateHeight(node);

    // TODO : Implement balance
    // AvlTree_balance(node);

    return AVL_TREE_E_OK;
}


/*--------------------------------------------- Traverse through AVL Tree --------------------------------------------*/

// TODO : Implement a non-recursive DFS
static void AvlTree_dfs(AvlTreeNode *node, AvlTreeTraverserF traverserF) {
    if (node->left != NULL) AvlTree_dfs(node->left, traverserF);
    traverserF(node);
    if (node->right != NULL) AvlTree_dfs(node->right, traverserF);
}

static AvlTreeErrCode AvlTree_traverse(AvlTree *this, AvlTreeTraverserF traverserF) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (traverserF == NULL) return AVL_TREE_E_NULL_ARG;


    if (this->_tree != NULL) AvlTree_dfs(this->_tree, traverserF);

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
