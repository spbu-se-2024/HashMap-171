#include "multiset.h"
#include "../hash-functions/hash-function.h"

#include <string.h>


/*------------------------------------------------------- Utils ------------------------------------------------------*/

static MultisetErrCode Multiset_convertAvlTreeErrCode(AvlTreeErrCode avlTreeErrCode) {
    switch (avlTreeErrCode) {
        case AVL_TREE_E_OK:
            return MULTISET_E_OK;
        case AVL_TREE_E_MEM_ALLOC:
            return MULTISET_E_MEM_ALLOC;
        case AVL_TREE_E_NULL_THIS:
        case AVL_TREE_E_NULL_ARG:
            return MULTISET_E_NULL_ARG;
        case AVL_TREE_E_OTHER:
        default:
            return MULTISET_E_OTHER;
    }
}


/*------------------------------------------------- Hash conversions -------------------------------------------------*/

static size_t hashSHA_1(String word) {
    unsigned int buffer[5] = {0};

    SHA_1(word, strlen(word), buffer);

    return ((unsigned long long) buffer[1]) << 32 | ((unsigned long long) buffer[0]);
}

static size_t getHash(String key, MultisetHashFuncLabel multisetHashFuncLabel) {
    // TODO : Add other hash functions
    switch (multisetHashFuncLabel) {
        case MULTISET_HASH_FUNC_LABEL_SHA_1:
            return hashSHA_1(key);
        default:
            return 0;
    }
}

static inline size_t Multiset_getItemIndex(String item, MultisetConfig config) {
    return getHash(item, config.hashFuncLabel) % config.size;
}


/*----------------------------------------------- Has Item & Count Item ----------------------------------------------*/

static MultisetErrCode Multiset_hasItem(Multiset *this, String item, bool *hasItem) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;
    if (hasItem == NULL) return MULTISET_E_NULL_ARG;


    AvlTreeErrCode avlTreeErrCode;


    size_t index = Multiset_getItemIndex(item, this->_config);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    if ((avlTreeErrCode = pAvlTree->find(pAvlTree, item, &avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }


    *hasItem = avlTreeNode != NULL;

    return MULTISET_E_OK;
}


static MultisetErrCode Multiset_countItem(Multiset *this, String item, size_t *itemCount) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;
    if (itemCount == NULL) return MULTISET_E_NULL_ARG;


    AvlTreeErrCode avlTreeErrCode;


    size_t index = getHash(item, this->_config.hashFuncLabel) % this->_config.size;
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    if ((avlTreeErrCode = pAvlTree->find(pAvlTree, item, &avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }


    *itemCount = avlTreeNode != NULL ? avlTreeNode->count : 0;

    return MULTISET_E_OK;
}


/*----------------------------------------------------- Add Item -----------------------------------------------------*/

static MultisetErrCode Multiset_addItem(Multiset *this, String item) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;


    AvlTreeErrCode avlTreeErrCode;


    size_t index = Multiset_getItemIndex(item, this->_config);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    if ((avlTreeErrCode = pAvlTree->insert(pAvlTree, item, &avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }

    return MULTISET_E_OK;
}


/*---------------------------------------------------- Remove Item ---------------------------------------------------*/

static MultisetErrCode Multiset_removeItem(Multiset *this, String item) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;


    AvlTreeErrCode avlTreeErrCode;


    size_t index = Multiset_getItemIndex(item, this->_config);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    if ((avlTreeErrCode = pAvlTree->find(pAvlTree, item, &avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }

    if ((avlTreeErrCode = pAvlTree->delete(pAvlTree, avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }

    return MULTISET_E_OK;
}


/*----------------------------------------------------- Traverse -----------------------------------------------------*/

static MultisetTraverserF multisetTraverserF = NULL;

static void Multiset_avlTreeTraverserF(AvlTreeNode *node) { multisetTraverserF(node->item, node->count); }

static MultisetErrCode Multiset_traverse(Multiset *this, MultisetTraverserF traverser) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (traverser == NULL) return MULTISET_E_NULL_ARG;


    AvlTreeErrCode avlTreeErrCode;


    multisetTraverserF = traverser;

    const size_t multisetSize = this->_config.size;
    for (size_t i = 0; i < multisetSize; i++) {
        if ((avlTreeErrCode = this->_data[i].traverse(&this->_data[i], Multiset_avlTreeTraverserF))) {
            return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
        }
    }

    multisetTraverserF = NULL;

    return MULTISET_E_OK;
}


/*------------------------------------------------------- Clear ------------------------------------------------------*/

static MultisetErrCode Multiset_clear(Multiset *this) {
    if (this == NULL) return MULTISET_E_NULL_THIS;


    AvlTreeErrCode avlTreeErrCode;


    const size_t multisetSize = this->_config.size;
    for (size_t i = 0; i < multisetSize; i++) {
        if ((avlTreeErrCode = this->_data[i].clear(&this->_data[i]))) {
            return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
        }
    }

    return MULTISET_E_OK;
}
