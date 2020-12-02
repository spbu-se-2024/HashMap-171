#ifndef HASHMAP_171_AVL_TREE_H
#define HASHMAP_171_AVL_TREE_H

#include "avl-tree-err.h"


/*--------------------------------------------------- AVL Tree Node --------------------------------------------------*/

typedef struct avl_tree_node_t AvlTreeNode;

struct avl_tree_node_t {
    void *item;
    size_t count;
    size_t height;
    AvlTreeNode *parent;
    AvlTreeNode *left;
    AvlTreeNode *right;
};


typedef int (*AvlTreeItemCompF)(void *itemA, void *itemB);

typedef void (*AvlTreeItemFreeF)(void *item);


typedef void (*AvlTreeTraverserF)(void *externalData, AvlTreeNode *node);


/*----------------------------------------------------- AVL Tree -----------------------------------------------------*/

typedef struct avl_tree_t AvlTree;

struct avl_tree_t {
    AvlTreeNode *_tree;

    AvlTreeItemCompF _compF;

    AvlTreeItemFreeF _freeF;

    size_t itemsCount;

    size_t uniqueItemsCount;


    // AVL Tree Interface

    AvlTreeErrCode (*findItem)(AvlTree *this, void *item, AvlTreeNode **pItemNode);

    AvlTreeErrCode (*findClosestItem)(AvlTree *this, void *item, AvlTreeNode **pItemNode);

    AvlTreeErrCode (*prevNode)(AvlTree *this, AvlTreeNode *node, AvlTreeNode **pPrevNode);

    AvlTreeErrCode (*nextNode)(AvlTree *this, AvlTreeNode *node, AvlTreeNode **pNextNode);

    AvlTreeErrCode (*addItem)(AvlTree *this, void *item, AvlTreeNode **pNewNode);

    AvlTreeErrCode (*addItemTimes)(AvlTree *this, void *item, size_t times, AvlTreeNode **pNewNode);

    AvlTreeErrCode (*removeItem)(AvlTree *this, void *item);

    AvlTreeErrCode (*removeItemWithCopies)(AvlTree *this, void *item);


    AvlTreeErrCode (*traverse)(AvlTree *this, void *externalData, AvlTreeTraverserF traverserF);

    AvlTreeErrCode (*clear)(AvlTree *this);
};


/*-------------------------------------------- Memory Management Interface -------------------------------------------*/

AvlTreeErrCode AvlTree_initAvlTree(AvlTree *avlTree, AvlTreeItemCompF compF, AvlTreeItemFreeF freeF);

AvlTreeErrCode AvlTree_eraseAvlTree(AvlTree *avlTree);


AvlTreeErrCode AvlTree_allocAvlTree(AvlTree **pAvlTree, AvlTreeItemCompF compF, AvlTreeItemFreeF freeF);

AvlTreeErrCode AvlTree_freeAvlTree(AvlTree **pAvlTree);


#endif // HASHMAP_171_AVL_TREE_H
