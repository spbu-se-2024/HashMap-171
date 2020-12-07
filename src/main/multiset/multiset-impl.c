#include "multiset.h"
#include "../hash-functions/hash-function.h"

#include <string.h>


/*------------------------------------------------------- Utils ------------------------------------------------------*/

static MultisetErrCode Multiset_convertHashFuncErrCode(HashFuncErrCode hashFuncErrCode) {
    switch (hashFuncErrCode) {
        case HASH_FUNC_E_OK:
            return MULTISET_E_OK;
        case HASH_FUNC_E_MEM_ALLOC:
            return MULTISET_E_MEM_ALLOC;
        case HASH_FUNC_E_NULL_ARG:
            return MULTISET_E_NULL_ARG;
        case HASH_FUNC_E_OTHER:
        default:
            return MULTISET_E_OTHER;
    }
}

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

static MultisetErrCode convertMd5HashToSizeT(MultisetItem item, size_t *index) {
    HashFuncErrCode errCode;

    uint32_t hash[4];
    if ((errCode = calculateMd5Hash(item, strlen(item), hash))) {
        return Multiset_convertHashFuncErrCode(errCode);
    }

    size_t _index = 0;
    _index = (_index + hash[3] % SIZE_MAX) % SIZE_MAX;
    _index = (_index + ((uint64_t) hash[2] << 32u) % SIZE_MAX) % SIZE_MAX;
    *index = _index;

    return MULTISET_E_OK;
}

static MultisetErrCode convertPolynomialHashToSizeT(MultisetItem item, size_t *index) {
    HashFuncErrCode errCode;

    uint64_t hash;
    if ((errCode = calculatePolynomialHash(item, strlen(item), &hash))) {
        return Multiset_convertHashFuncErrCode(errCode);
    }

    *index = hash % SIZE_MAX;

    return MULTISET_E_OK;
}

static MultisetErrCode convertSha1HashToSizeT(MultisetItem item, size_t *index) {
    HashFuncErrCode errCode;

    uint32_t hash[5];
    if ((errCode = calculateSha1Hash(item, strlen(item), hash))) {
        return Multiset_convertHashFuncErrCode(errCode);
    }

    size_t _index = 0;
    _index = (_index + hash[4] % SIZE_MAX) % SIZE_MAX;
    _index = (_index + ((uint64_t) hash[3] << 32u) % SIZE_MAX) % SIZE_MAX;
    *index = _index;

    return MULTISET_E_OK;
}


static MultisetErrCode Multiset_getItemIndex(MultisetItem item, MultisetConfig config, size_t *index) {
    MultisetErrCode errCode;

    size_t _index;

    switch (config.hashFuncLabel) {
        case MULTISET_HASH_FUNC_LABEL_MD5:
            errCode = convertMd5HashToSizeT(item, &_index);
            break;
        case MULTISET_HASH_FUNC_LABEL_POLYNOMIAL:
            errCode = convertPolynomialHashToSizeT(item, &_index);
            break;
        case MULTISET_HASH_FUNC_LABEL_SHA_1:
            errCode = convertSha1HashToSizeT(item, &_index);
            break;
        default:
            return MULTISET_E_OTHER;
    }

    if (errCode) return errCode;

    *index = _index % config.size;

    return MULTISET_E_OK;
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

    MultisetErrCode multisetErrCode;
    AvlTreeErrCode avlTreeErrCode;


    size_t index;
    if ((multisetErrCode = Multiset_getItemIndex(item, this->_config, &index))) {
        return multisetErrCode;
    }
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

    MultisetErrCode multisetErrCode;
    AvlTreeErrCode avlTreeErrCode;


    if (times == 0) return MULTISET_E_OK;

    size_t index;
    if ((multisetErrCode = Multiset_getItemIndex(item, this->_config, &index))) {
        return multisetErrCode;
    }
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

    MultisetErrCode multisetErrCode;
    AvlTreeErrCode avlTreeErrCode;


    size_t index;
    if ((multisetErrCode = Multiset_getItemIndex(item, this->_config, &index))) {
        return multisetErrCode;
    }
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

    MultisetErrCode multisetErrCode;
    AvlTreeErrCode avlTreeErrCode;


    size_t index;
    if ((multisetErrCode = Multiset_getItemIndex(item, this->_config, &index))) {
        return multisetErrCode;
    }
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
                avlTreeErrCode = this->_data[i].traverse(&this->_data[i], &layeredExternalData,
                                                         Multiset_avlTreeTraverserF)
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

static void Multiset_getStatsMultisetTraverser(void *externalData, MultisetItem item, size_t count) {
    MultisetStats *stats = externalData;

    if (count > stats->maxCount) {
        stats->maxCount = count;
        stats->maxCountWord = item;
    }
}

static MultisetErrCode Multiset_getStatistics(Multiset *this, MultisetStats *stats) {
    if (this == NULL) return MULTISET_E_NULL_THIS;
    if (stats == NULL) return MULTISET_E_NULL_ARG;

    MultisetErrCode errCode;

    stats->config = this->_config;
    stats->itemsCount = this->itemsCount;
    stats->uniqueItemsCount = this->uniqueItemsCount;
    stats->maxCount = 0;
    stats->maxCountWord = NULL;

    if ((errCode = this->traverse(this, stats, Multiset_getStatsMultisetTraverser)) != MULTISET_E_OK) {
        return errCode;
    }

    return MULTISET_E_OK;
}
