#include "avl-tree.h"

#include <stdlib.h>


/*------------------------------------------------------- Utils ------------------------------------------------------*/

static inline size_t AvlTree_maxSizeT(size_t a, size_t b) { return a < b ? b : a; }

static inline size_t AvlTree_getNodeHeight(AvlTreeNode *node) { return node != NULL ? node->height : 0; }


/*------------------------------------------------ Find Min & Max Item -----------------------------------------------*/

static AvlTreeErrCode AvlTree_findMinItem(AvlTree *this, AvlTreeNode **pMinItemNode) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(pMinItemNode == NULL, AVL_TREE_E_NULL_ARG,);


    AvlTreeNode *node = this->_tree;
    if (node != NULL) while (node->left != NULL) node = node->left;

    *pMinItemNode = node;

    return AVL_TREE_E_OK;
}

static AvlTreeErrCode AvlTree_findMaxItem(AvlTree *this, AvlTreeNode **pMaxItemNode) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(pMaxItemNode == NULL, AVL_TREE_E_NULL_ARG,);


    AvlTreeNode *node = this->_tree;
    if (node != NULL) while (node->right != NULL) node = node->right;

    *pMaxItemNode = node;

    return AVL_TREE_E_OK;
}


/*------------------------------------------------- Balance AVL Tree -------------------------------------------------*/

static inline void AvlTree_updateHeight(AvlTreeNode *node) {
    node->height = AvlTree_maxSizeT(AvlTree_getNodeHeight(node->left), AvlTree_getNodeHeight(node->right)) + 1;
}

static void AvlTree_updateHeightRecursively(AvlTreeNode *node) {
    if (node == NULL) return;
    AvlTree_updateHeight(node);
    AvlTree_updateHeightRecursively(node->parent);
}


static inline void AvlTree_rotateLeft(AvlTreeNode *node) {
    AvlTreeNode *nodeP = node->parent;
    AvlTreeNode *nodeA = node;
    AvlTreeNode *nodeB = node->right;
    AvlTreeNode *treeB = node->right->left;


    nodeA->parent = nodeB;
    nodeB->left = nodeA;

    nodeB->parent = nodeP;
    if (nodeP != NULL) *(nodeP->left == nodeA ? &nodeP->left : &nodeP->right) = nodeB;

    nodeA->right = treeB;
    if (treeB != NULL) treeB->parent = nodeA;

    AvlTree_updateHeightRecursively(nodeA);
}

static inline void AvlTree_rotateRight(AvlTreeNode *node) {
    AvlTreeNode *nodeP = node->parent;
    AvlTreeNode *nodeA = node;
    AvlTreeNode *nodeB = node->left;
    AvlTreeNode *treeB = node->left->right;


    nodeA->parent = nodeB;
    nodeB->right = nodeA;

    nodeB->parent = nodeP;
    if (nodeP != NULL) *(nodeP->left == nodeA ? &nodeP->left : &nodeP->right) = nodeB;

    nodeA->left = treeB;
    if (treeB != NULL) treeB->parent = nodeA;

    AvlTree_updateHeightRecursively(nodeA);
}

static void AvlTree_balance(AvlTree *avlTree, AvlTreeNode *node) {
    if (node == NULL) return;

    size_t lChH = AvlTree_getNodeHeight(node->left);
    size_t rChH = AvlTree_getNodeHeight(node->right);
    if (lChH + 2 == rChH) {
        size_t rChLChH = AvlTree_getNodeHeight(node->right->left);
        size_t rChRChH = AvlTree_getNodeHeight(node->right->right);

        if (rChLChH == rChRChH + 1) AvlTree_rotateRight(node->right);
        AvlTree_rotateLeft(node);
    } else if (rChH + 2 == lChH) {
        size_t lChLChH = AvlTree_getNodeHeight(node->left->left);
        size_t lChRChH = AvlTree_getNodeHeight(node->left->right);

        if (lChRChH == lChLChH + 1) AvlTree_rotateLeft(node->left);
        AvlTree_rotateRight(node);
    }

    if (node->parent != NULL) AvlTree_balance(avlTree, node->parent);
    else avlTree->_tree = node;
}



/*--------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ AVL Tree Interface ------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------- Find Item ----------------------------------------------------*/

static AvlTreeErrCode AvlTree_findItem(AvlTree *this, void *item, AvlTreeNode **pItemNode) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(item == NULL, AVL_TREE_E_NULL_ARG,);
    AvlTree_autoprintErrAndStopRunIf(pItemNode == NULL, AVL_TREE_E_NULL_ARG,);


    AvlTreeNode *node;
    AvlTree_stopRunOnBadErrCode(this->findClosestItem(this, item, &node),);

    *pItemNode = node != NULL && this->_compF(item, node->item) == 0 ? node : NULL;

    return AVL_TREE_E_OK;
}

static AvlTreeErrCode AvlTree_findClosestItem(AvlTree *this, void *item, AvlTreeNode **pItemNode) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(item == NULL, AVL_TREE_E_NULL_ARG,);
    AvlTree_autoprintErrAndStopRunIf(pItemNode == NULL, AVL_TREE_E_NULL_ARG,);


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
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(node == NULL, AVL_TREE_E_NULL_ARG,);
    AvlTree_autoprintErrAndStopRunIf(pPrevNode == NULL, AVL_TREE_E_NULL_ARG,);


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
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(node == NULL, AVL_TREE_E_NULL_ARG,);
    AvlTree_autoprintErrAndStopRunIf(pNextNode == NULL, AVL_TREE_E_NULL_ARG,);


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
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(item == NULL, AVL_TREE_E_NULL_ARG,);


    AvlTree_stopRunOnBadErrCode(this->addItemTimes(this, item, 1, pNewNode),);

    return AVL_TREE_E_OK;
}

static AvlTreeErrCode AvlTree_addItemTimes(AvlTree *this, void *item, size_t times, AvlTreeNode **pNewNode) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(item == NULL, AVL_TREE_E_NULL_ARG,);


    if (times == 0) return AVL_TREE_E_OK;

    AvlTreeNode *node;
    AvlTree_stopRunOnBadErrCode(this->findClosestItem(this, item, &node),);

    AvlTreeNode *newNode;
    if (node == NULL) {
        newNode = malloc(sizeof(AvlTreeNode));
        AvlTree_autoprintErrAndStopRunIf(newNode == NULL, AVL_TREE_E_MEM_ALLOC,);

        *newNode = (AvlTreeNode) {item, .count = times, .height = 1};

        this->_tree = newNode;

        this->itemsCount = times;
        this->uniqueItemsCount = 1;
    } else {
        int comp = this->_compF(item, node->item);

        if (comp == 0) {
            newNode = node;

            newNode->count += times;
            if (this->_freeF != NULL) this->_freeF(item);

            this->itemsCount += times;
        } else {
            newNode = *(comp < 0 ? &node->left : &node->right) = malloc(sizeof(AvlTreeNode));
            AvlTree_autoprintErrAndStopRunIf(newNode == NULL, AVL_TREE_E_MEM_ALLOC,);

            *newNode = (AvlTreeNode) {item, .count = times, .height = 1, .parent = node};

            AvlTree_updateHeightRecursively(newNode);
            AvlTree_balance(this, newNode);

            this->itemsCount += times;
            this->uniqueItemsCount += 1;
        }
    }

    if (pNewNode != NULL) *pNewNode = newNode;

    return AVL_TREE_E_OK;
}


/*---------------------------------------------------- Remove Item ---------------------------------------------------*/

static AvlTreeErrCode AvlTree_removeNode(AvlTree *this, AvlTreeNode *node) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);


    if (node == NULL) return AVL_TREE_E_OK;

    AvlTreeNode *balancePoint;

    AvlTreeNode *replace;
    if (node->left == NULL) {
        replace = node->right;

        if (replace != NULL) replace->parent = node->parent;

        if (node->parent != NULL) {
            *(node == node->parent->left ? &node->parent->left : &node->parent->right) = replace;
        }


        balancePoint = replace;
    } else {
        AvlTree_stopRunOnBadErrCode(this->prevNode(this, node, &replace),);

        if (replace == node->left) {
            replace->parent = node->parent;
            replace->right = node->right;

            if (node->parent != NULL) {
                *(node == node->parent->left ? &node->parent->left : &node->parent->right) = replace;
            }
            if (node->right != NULL) node->right->parent = replace;


            balancePoint = replace;
        } else {
            // Replace left child
            AvlTreeNode *replaceLeftChild = replace->left;

            if (replaceLeftChild != NULL) replaceLeftChild->parent = replace->parent;

            *(replace == replace->parent->left ? &replace->parent->left : &replace->parent->right) = replaceLeftChild;


            // Replace
            replace->parent = node->parent;
            replace->left = node->left;
            replace->right = node->right;

            if (node->parent != NULL) {
                *(node == node->parent->left ? &node->parent->left : &node->parent->right) = replace;
            }
            node->left->parent = replace;
            if (node->right != NULL) node->right->parent = replace;


            balancePoint = replaceLeftChild;
        }
    }

    if (node == this->_tree) this->_tree = replace;

    if (this->_freeF != NULL) this->_freeF(node->item);
    free(node);

    AvlTree_updateHeightRecursively(balancePoint);
    AvlTree_balance(this, balancePoint);

    return AVL_TREE_E_OK;
}


static AvlTreeErrCode AvlTree_removeItem(AvlTree *this, void *item) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(item == NULL, AVL_TREE_E_NULL_ARG,);


    AvlTreeNode *node;
    AvlTree_stopRunOnBadErrCode(this->findItem(this, item, &node),);

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
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(item == NULL, AVL_TREE_E_NULL_ARG,);


    AvlTreeNode *node;
    AvlTree_stopRunOnBadErrCode(this->findItem(this, item, &node),);

    if (node != NULL) {
        this->itemsCount -= node->count;
        --this->uniqueItemsCount;
    }

    return AvlTree_removeNode(this, node);
}


/*--------------------------------------------- Traverse Through AVL Tree --------------------------------------------*/

static void AvlTree_dfs(AvlTreeNode *node, void *externalData, AvlTreeTraverserF traverserF) {
    if (node->left != NULL) AvlTree_dfs(node->left, externalData, traverserF);
    traverserF(externalData, node);
    if (node->right != NULL) AvlTree_dfs(node->right, externalData, traverserF);
}

static AvlTreeErrCode AvlTree_traverse(AvlTree *this, void *externalData, AvlTreeTraverserF traverserF) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);
    AvlTree_autoprintErrAndStopRunIf(traverserF == NULL, AVL_TREE_E_NULL_ARG,);


    if (this->_tree != NULL) AvlTree_dfs(this->_tree, externalData, traverserF);

    return AVL_TREE_E_OK;
}


/*-------------------------------------------------- Clear AVL Tree --------------------------------------------------*/

static void AvlTree_recursiveNodeFree(AvlTreeNode *node, AvlTreeItemFreeF freeF) {
    if (node->left != NULL) AvlTree_recursiveNodeFree(node->left, freeF);
    if (node->right != NULL) AvlTree_recursiveNodeFree(node->right, freeF);

    if (freeF != NULL) freeF(node->item);
    free(node);
}

static AvlTreeErrCode AvlTree_clear(AvlTree *this) {
    AvlTree_autoprintErrAndStopRunIf(this == NULL, AVL_TREE_E_NULL_THIS,);


    if (this->_tree != NULL) {
        AvlTree_recursiveNodeFree(this->_tree, this->_freeF);
        this->_tree = NULL;
    }

    this->itemsCount = 0;
    this->uniqueItemsCount = 0;

    return AVL_TREE_E_OK;
}
