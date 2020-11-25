#ifndef HASHMAP_171_BST_H
#define HASHMAP_171_BST_H

#include "bst_err.h"


// PREFIX - BST / Bst


/*------------------------------------------------ Binary Search Tree ------------------------------------------------*/

typedef struct {
    void *parent;
    void *left;
    void *right;
    void *value;
} BstNode;


/*------------------------------------------------ Binary Search Tree ------------------------------------------------*/

typedef void (*CopyGenericObjectF)(void *object);

typedef void (*FreeGenericObjectF)(void *object);

typedef void (*ProcessGenericObjectF)(void *object);


typedef struct {
    void *_tree;

    FreeGenericObjectF _freeObjectF;

    CopyGenericObjectF _copyObjectF;


    // Binary Search Tree Interface

    /** Find element. */
    BstErrCode (*find)(void);

    /** Find next element. */
    BstErrCode (*next)(void);

    /** Insert new element. */
    BstErrCode (*insert)(void);

    /** Delete element. */
    BstErrCode (*delete)(void);

    /** Traverse through all of the elements of the BST. */
    BstErrCode (*traverse)(void);


    /** Clear BST */
    BstErrCode (*clear)(void);
} BinarySearchTree;


/*-------------------------------------------- Memory Management Interface -------------------------------------------*/

BstErrCode Bst_initBinarySearchTree(BinarySearchTree *bst);

BstErrCode Bst_eraseBinarySearchTree(BinarySearchTree *bst);


BstErrCode Bst_newBinarySearchTree(BinarySearchTree **bst);

BstErrCode Bst_freeBinarySearchTree(BinarySearchTree **bst);


#endif // HASHMAP_171_BST_H
