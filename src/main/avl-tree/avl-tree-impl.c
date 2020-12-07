#include "avl-tree.h"

#include <stdbool.h>
#include <stdlib.h>


/*------------------------------------------------------- Utils ------------------------------------------------------*/

static inline size_t AvlTree_maxSizeT(size_t a, size_t b) { return a < b ? b : a; }

static inline size_t AvlTree_getNodeHeight(AvlTreeNode *node) { return node != NULL ? node->height : 0; }


/*------------------------------------------------ Find Min & Max Item -----------------------------------------------*/

static AvlTreeErrCode AvlTree_findMinItem(AvlTree *this, AvlTreeNode **pMinItemNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (pMinItemNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *node = this->_tree;
    if (node != NULL) while (node->left != NULL) node = node->left;

    *pMinItemNode = node;

    return AVL_TREE_E_OK;
}

static AvlTreeErrCode AvlTree_findMaxItem(AvlTree *this, AvlTreeNode **pMaxItemNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (pMaxItemNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *node = this->_tree;
    if (node != NULL) while (node->right != NULL) node = node->right;

    *pMaxItemNode = node;

    return AVL_TREE_E_OK;
}


/*------------------------------------------------- Balance AVL Tree -------------------------------------------------*/

static void AvlTree_updateHeight(AvlTreeNode *node) {
    if (node == NULL) return;
    node->height = AvlTree_maxSizeT(AvlTree_getNodeHeight(node->left), AvlTree_getNodeHeight(node->right)) + 1;
    AvlTree_updateHeight(node->parent);
}

// TODO : Implement balance
static void AvlTree_balance(AvlTreeNode *node) {
    (void) node;
}



/*--------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ AVL Tree Interface ------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------- Find Item ----------------------------------------------------*/

static AvlTreeErrCode AvlTree_findItem(AvlTree *this, void *item, AvlTreeNode **pItemNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;
    if (pItemNode == NULL) return AVL_TREE_E_NULL_ARG;

    AvlTreeErrCode errCode;


    AvlTreeNode *node;
    if ((errCode = this->findClosestItem(this, item, &node))) {
        return errCode;
    }

    *pItemNode = node != NULL && this->_compF(item, node->item) == 0 ? node : NULL;

    return AVL_TREE_E_OK;
}

static AvlTreeErrCode AvlTree_findClosestItem(AvlTree *this, void *item, AvlTreeNode **pItemNode) {
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

static AvlTreeErrCode AvlTree_prevNode(AvlTree *this, AvlTreeNode *node, AvlTreeNode **pPrevNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (node == NULL) return AVL_TREE_E_NULL_ARG;
    if (pPrevNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *minItemNode;
    AvlTree_findMinItem(this, &minItemNode);

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

static AvlTreeErrCode AvlTree_nextNode(AvlTree *this, AvlTreeNode *node, AvlTreeNode **pNextNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (node == NULL) return AVL_TREE_E_NULL_ARG;
    if (pNextNode == NULL) return AVL_TREE_E_NULL_ARG;


    AvlTreeNode *maxItemNode;
    AvlTree_findMaxItem(this, &maxItemNode);

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


/*----------------------------------------------------- Add Item -----------------------------------------------------*/

static AvlTreeErrCode AvlTree_addItem(AvlTree *this, void *item, AvlTreeNode **pNewNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;

    AvlTreeErrCode errCode;


    if ((errCode = this->addItemTimes(this, item, 1, pNewNode))) {
        return errCode;
    }

    return AVL_TREE_E_OK;
}

// TODO : Rewrite AvlTree_addItemTimes(...)
static AvlTreeErrCode AvlTree_addItemTimes(AvlTree *this, void *item, size_t times, AvlTreeNode **pNewNode) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;

    AvlTreeErrCode errCode;


    if (times == 0) return AVL_TREE_E_OK;

    AvlTreeNode *node;
    if ((errCode = this->findClosestItem(this, item, &node))) {
        return errCode;
    }

    if (node == NULL) {
        this->_tree = malloc(sizeof(AvlTreeNode));
        if (this->_tree == NULL) {
            return AVL_TREE_E_MEM_ALLOC;
        }

        *this->_tree = (AvlTreeNode) {item, .count = times, .height = 1};

        node = this->_tree;
    } else {
        int comp = this->_compF(item, node->item);

        if (comp < 0) {
            node->left = malloc(sizeof(AvlTreeNode));
            if (node->left == NULL) return AVL_TREE_E_MEM_ALLOC;

            *node->left = (AvlTreeNode) {item, .count = times, .height = 1, .parent = node};

            node = node->left;
        } else if (comp == 0) {
            node->count += times;
            if (this->_freeF != NULL) this->_freeF(item);
        } else {
            node->right = malloc(sizeof(AvlTreeNode));
            if (node->right == NULL) return AVL_TREE_E_MEM_ALLOC;

            *node->right = (AvlTreeNode) {item, .count = times, .height = 1, .parent = node};

            node = node->right;
        }
    }

    if (pNewNode != NULL) *pNewNode = node;


    AvlTree_updateHeight(node);
    AvlTree_balance(node);

    return AVL_TREE_E_OK;
}


/*---------------------------------------------------- Remove Item ---------------------------------------------------*/

// TODO : Rewrite AvlTree_removeNode(...)
static AvlTreeErrCode AvlTree_removeNode(AvlTree *this, AvlTreeNode *node) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;


    if (node == NULL) return AVL_TREE_E_OK;

    AvlTreeNode *replace = node->left;
    if (replace != NULL) while (replace->right != NULL) replace = replace->right;

    if (replace != NULL) {
        replace->right = node->right;
        if (node->right != NULL) node->right->parent = replace;

        node->left->parent = node->parent;
        if (node->parent != NULL) {
            if (node->parent->left == node) node->parent->left = node->left;
            else node->parent->right = node->left;
        } else {
            this->_tree = node->left;
        }
    } else {
        replace = node->right;

        if (node->right != NULL) node->right->parent = node->parent;
        if (node->parent != NULL) {
            if (node->parent->left == node) node->parent->left = node->right;
            else node->parent->right = node->right;
        } else {
            this->_tree = node->right;
        }
    }

    if (this->_freeF != NULL) this->_freeF(node->item);
    free(node);


    AvlTree_updateHeight(replace);
    AvlTree_balance(replace);

    return AVL_TREE_E_OK;
}


static AvlTreeErrCode AvlTree_removeItem(AvlTree *this, void *item) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;

    AvlTreeErrCode errCode;


    AvlTreeNode *node;
    if ((errCode = this->findItem(this, item, &node))) {
        return errCode;
    }

    if (node != NULL && node->count > 1) {
        --node->count;
        --this->itemsCount;
    } else {
        --this->itemsCount;
        --this->uniqueItemsCount;
        return AvlTree_removeNode(this, node);
    }

    return AVL_TREE_E_OK;
}

static AvlTreeErrCode AvlTree_removeItemWithCopies(AvlTree *this, void *item) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (item == NULL) return AVL_TREE_E_NULL_ARG;

    AvlTreeErrCode errCode;


    AvlTreeNode *node;
    if ((errCode = this->findItem(this, item, &node))) {
        return errCode;
    }

    if (node != NULL) {
        this->itemsCount -= node->count;
        --this->uniqueItemsCount;
    }

    return AvlTree_removeNode(this, node);
}


/*--------------------------------------------- Traverse Through AVL Tree --------------------------------------------*/

// TODO : Implement a non-recursive DFS
static void AvlTree_dfs(AvlTreeNode *node, void *externalData, AvlTreeTraverserF traverserF) {
    if (node->left != NULL) AvlTree_dfs(node->left, externalData, traverserF);
    traverserF(externalData, node);
    if (node->right != NULL) AvlTree_dfs(node->right, externalData, traverserF);
}

static AvlTreeErrCode AvlTree_traverse(AvlTree *this, void *externalData, AvlTreeTraverserF traverserF) {
    if (this == NULL) return AVL_TREE_E_NULL_THIS;
    if (traverserF == NULL) return AVL_TREE_E_NULL_ARG;


    if (this->_tree != NULL) AvlTree_dfs(this->_tree, externalData, traverserF);

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

    this->itemsCount = 0;
    this->uniqueItemsCount = 0;

    return AVL_TREE_E_OK;
}
