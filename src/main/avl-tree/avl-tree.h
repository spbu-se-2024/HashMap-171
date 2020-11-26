#ifndef HASHMAP_171_AVL_TREE_H
#define HASHMAP_171_AVL_TREE_H

#include "avl-tree-err.h"


/*--------------------------------------------------- AVL Tree Node --------------------------------------------------*/

typedef struct avl_tree_node_t AvlTreeNode;

struct avl_tree_node_t {
    void *item;
    size_t height;
    AvlTreeNode *parent;
    AvlTreeNode *left;
    AvlTreeNode *right;
};


/*----------------------------------------------------- AVL Tree -----------------------------------------------------*/

typedef int (*AvlTreeItemCompF)(void *itemA, void *itemB);

typedef void (*AvlTreeItemFreeF)(void *item);


typedef void (*AvlTreeTraverserF)(void *item);


typedef struct avl_tree_t AvlTree;

struct avl_tree_t {
    AvlTreeNode *_tree;

    AvlTreeItemCompF _compF;

    AvlTreeItemFreeF _freeF;


    // AVL Tree Interface

    AvlTreeErrCode (*find)(AvlTree *this, void *item, AvlTreeNode **pItemNode);

    AvlTreeErrCode (*next)(AvlTree *this, AvlTreeNode *node, AvlTreeNode **pNextNode);

    AvlTreeErrCode (*insert)(AvlTree *this, void *item, AvlTreeNode **pNewNode);

    AvlTreeErrCode (*delete)(AvlTree *this, AvlTreeNode *node);


    AvlTreeErrCode (*traverse)(AvlTree *this, AvlTreeTraverserF traverserF);

    AvlTreeErrCode (*clear)(AvlTree *this);
};


/*-------------------------------------------- Memory Management Interface -------------------------------------------*/

AvlTreeErrCode AvlTree_initAvlTree(AvlTree *avlTree, AvlTreeItemCompF compF, AvlTreeItemFreeF freeF);

AvlTreeErrCode AvlTree_eraseAvlTree(AvlTree *avlTree);


AvlTreeErrCode AvlTree_allocAvlTree(AvlTree **pAvlTree, AvlTreeItemCompF compF, AvlTreeItemFreeF freeF);

AvlTreeErrCode AvlTree_freeAvlTree(AvlTree **pAvlTree);


#endif // HASHMAP_171_AVL_TREE_H
