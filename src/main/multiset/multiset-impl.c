#include "multiset.h"
#include "../hash-functions/hash-function.h"


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

// TODO : Implement convertMd5HashToSizeT(...)
static size_t convertMd5HashToSizeT(MultisetItem item) {
    return 0;
}

// TODO : Implement convertPolynomialHashToSizeT(...)
static size_t convertPolynomialHashToSizeT(MultisetItem item) {
    return 0;
}

// TODO : Implement convertSha1HashToSizeT(...)
static size_t convertSha1HashToSizeT(MultisetItem item) {
    return 0;
}


static size_t getConvertedHash(MultisetItem item, MultisetHashFuncLabel multisetHashFuncLabel) {
    switch (multisetHashFuncLabel) {
        case MULTISET_HASH_FUNC_LABEL_MD5:
            return convertMd5HashToSizeT(item);
        case MULTISET_HASH_FUNC_LABEL_POLYNOMIAL:
            return convertPolynomialHashToSizeT(item);
        case MULTISET_HASH_FUNC_LABEL_SHA_1:
            return convertSha1HashToSizeT(item);
        default:
            return 0;
    }
}

static inline size_t Multiset_getItemIndex(MultisetItem item, MultisetConfig config) {
    return getConvertedHash(item, config.hashFuncLabel) % config.size;
}



/*--------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Multiset Interface ------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------- Has Item & Count Item ----------------------------------------------*/

static MultisetErrCode Multiset_hasItem(Multiset *this, MultisetItem item, bool *hasItem) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;
    if (hasItem == NULL) return MULTISET_E_NULL_ARG;

    MultisetErrCode errCode;


    size_t itemCount;
    if ((errCode = this->countItem(this, item, &itemCount))) {
        return errCode;
    }

    *hasItem = itemCount != 0;

    return MULTISET_E_OK;
}

static MultisetErrCode Multiset_countItem(Multiset *this, MultisetItem item, size_t *itemCount) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;
    if (itemCount == NULL) return MULTISET_E_NULL_ARG;

    AvlTreeErrCode avlTreeErrCode;


    size_t index = Multiset_getItemIndex(item, this->_config);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    if ((avlTreeErrCode = pAvlTree->findItem(pAvlTree, (void *) item, &avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }

    *itemCount = avlTreeNode != NULL ? avlTreeNode->count : 0;

    return MULTISET_E_OK;
}


/*----------------------------------------------------- Add Item -----------------------------------------------------*/

static MultisetErrCode Multiset_addItem(Multiset *this, MultisetItem item) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;

    MultisetErrCode errCode;


    if ((errCode = this->addItemTimes(this, item, 1))) {
        return errCode;
    }

    return MULTISET_E_OK;
}

static MultisetErrCode Multiset_addItemTimes(Multiset *this, MultisetItem item, size_t times) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;

    AvlTreeErrCode avlTreeErrCode;


    if (times == 0) return MULTISET_E_OK;

    size_t index = Multiset_getItemIndex(item, this->_config);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    if ((avlTreeErrCode = pAvlTree->addItemTimes(pAvlTree, (void *) item, times, &avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }

    this->itemsCount += times;
    if (avlTreeNode->count == times) ++this->uniqueItemsCount;

    return MULTISET_E_OK;
}


/*---------------------------------------------------- Remove Item ---------------------------------------------------*/

static MultisetErrCode Multiset_removeItem(Multiset *this, MultisetItem item) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;

    AvlTreeErrCode avlTreeErrCode;


    size_t index = Multiset_getItemIndex(item, this->_config);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    if ((avlTreeErrCode = pAvlTree->findItem(pAvlTree, (void *) item, &avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }

    if (avlTreeNode != NULL) {
        size_t itemCount = avlTreeNode->count;

        if ((avlTreeErrCode = pAvlTree->removeItem(pAvlTree, (void *) item))) {
            return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
        }

        --this->itemsCount;
        if (itemCount == 1) --this->uniqueItemsCount;
    }

    return MULTISET_E_OK;
}

static MultisetErrCode Multiset_removeItemWithCopies(Multiset *this, MultisetItem item) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (item == NULL) return MULTISET_E_NULL_ARG;

    AvlTreeErrCode avlTreeErrCode;


    size_t index = Multiset_getItemIndex(item, this->_config);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    if ((avlTreeErrCode = pAvlTree->findItem(pAvlTree, (void *) item, &avlTreeNode))) {
        return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
    }

    if (avlTreeNode != NULL) {
        size_t itemCount = avlTreeNode->count;

        if ((avlTreeErrCode = pAvlTree->removeItemWithCopies(pAvlTree, (void *) item))) {
            return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
        }

        this->itemsCount -= itemCount;
        --this->uniqueItemsCount;
    }

    return MULTISET_E_OK;
}


/*----------------------------------------------------- Traverse -----------------------------------------------------*/

struct layered_external_data_t {
    void *externalData;
    MultisetTraverserF multisetTraverserF;
};

static void Multiset_avlTreeTraverserF(void *externalData, AvlTreeNode *node) {
    struct layered_external_data_t *layeredExternalData = externalData;
    layeredExternalData->multisetTraverserF(layeredExternalData->externalData, node->item, node->count);
}

static MultisetErrCode Multiset_traverse(Multiset *this, void *externalData, MultisetTraverserF traverser) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (traverser == NULL) return MULTISET_E_NULL_ARG;

    AvlTreeErrCode avlTreeErrCode;


    struct layered_external_data_t layeredExternalData = {externalData, traverser};

    const size_t multisetSize = this->_config.size;
    for (size_t i = 0; i < multisetSize; i++) {
        if ((
            avlTreeErrCode = this->_data[i].traverse(&this->_data[i], &layeredExternalData, Multiset_avlTreeTraverserF)
        )) {
            return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
        }
    }

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

    this->itemsCount = 0;
    this->uniqueItemsCount = 0;

    return MULTISET_E_OK;
}


/*-------------------------------------------------- Get Statistics --------------------------------------------------*/

// TODO : Implement Multiset_getStatistics(...)
static MultisetErrCode Multiset_getStatistics(Multiset *this, MultisetStats *stats) {
    return MULTISET_E_OK;
}
