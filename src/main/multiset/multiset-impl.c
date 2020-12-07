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

static MultisetErrCode Multiset_convertMd5HashToSizeT(MultisetItem item, size_t *index) {
    uint32_t hash[4];
    Multiset_stopRunOnBadErrCode(Multiset_convertHashFuncErrCode(
        HashFunc_calcMd5Hash(item, strlen(item), hash)
    ),);

    size_t _index = 0;
    _index = (_index + hash[3] % SIZE_MAX) % SIZE_MAX;
    _index = (_index + ((uint64_t) hash[2] << 32u) % SIZE_MAX) % SIZE_MAX;
    *index = _index;

    return MULTISET_E_OK;
}

static MultisetErrCode Multiset_convertPolynomialHashToSizeT(MultisetItem item, size_t *index) {
    uint64_t hash;
    Multiset_stopRunOnBadErrCode(Multiset_convertHashFuncErrCode(
        HashFunc_calcPolynomialHash(item, strlen(item), &hash)
    ),);

    *index = hash % SIZE_MAX;

    return MULTISET_E_OK;
}

static MultisetErrCode Multiset_convertSha1HashToSizeT(MultisetItem item, size_t *index) {
    uint32_t hash[5];
    Multiset_stopRunOnBadErrCode(Multiset_convertHashFuncErrCode(
        HashFunc_calcSha1Hash(item, strlen(item), hash)
    ),);

    size_t _index = 0;
    _index = (_index + hash[4] % SIZE_MAX) % SIZE_MAX;
    _index = (_index + ((uint64_t) hash[3] << 32u) % SIZE_MAX) % SIZE_MAX;
    *index = _index;

    return MULTISET_E_OK;
}


static MultisetErrCode Multiset_getItemIndex(MultisetItem item, MultisetConfig config, size_t *index) {
    size_t _index;

    switch (config.hashFuncLabel) {
        case MULTISET_HASH_FUNC_LABEL_MD5:
            Multiset_stopRunOnBadErrCode(Multiset_convertMd5HashToSizeT(item, &_index),);
            break;
        case MULTISET_HASH_FUNC_LABEL_POLYNOMIAL:
            Multiset_stopRunOnBadErrCode(Multiset_convertPolynomialHashToSizeT(item, &_index),);
            break;
        case MULTISET_HASH_FUNC_LABEL_SHA_1:
            Multiset_stopRunOnBadErrCode(Multiset_convertSha1HashToSizeT(item, &_index),);
            break;
        default:
            return MULTISET_E_OTHER;
    }

    *index = _index % config.size;

    return MULTISET_E_OK;
}



/*--------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------ Multiset Interface ------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------- Has Item & Count Item ----------------------------------------------*/

static MultisetErrCode Multiset_hasItem(Multiset *this, MultisetItem item, bool *hasItem) {
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);
    Multiset_autoprintErrAndStopRunIf(item == NULL, MULTISET_E_NULL_ARG,);
    Multiset_autoprintErrAndStopRunIf(hasItem == NULL, MULTISET_E_NULL_ARG,);


    size_t itemCount;
    Multiset_stopRunOnBadErrCode(this->countItem(this, item, &itemCount),);

    *hasItem = itemCount != 0;

    return MULTISET_E_OK;
}

static MultisetErrCode Multiset_countItem(Multiset *this, MultisetItem item, size_t *itemCount) {
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);
    Multiset_autoprintErrAndStopRunIf(item == NULL, MULTISET_E_NULL_ARG,);
    Multiset_autoprintErrAndStopRunIf(itemCount == NULL, MULTISET_E_NULL_ARG,);


    size_t index;
    Multiset_stopRunOnBadErrCode(Multiset_getItemIndex(item, this->_config, &index),);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    Multiset_stopRunOnBadErrCode(Multiset_convertAvlTreeErrCode(
        pAvlTree->findItem(pAvlTree, (void *) item, &avlTreeNode)
    ),);

    *itemCount = avlTreeNode != NULL ? avlTreeNode->count : 0;

    return MULTISET_E_OK;
}


/*----------------------------------------------------- Add Item -----------------------------------------------------*/

static MultisetErrCode Multiset_addItem(Multiset *this, MultisetItem item) {
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);
    Multiset_autoprintErrAndStopRunIf(item == NULL, MULTISET_E_NULL_ARG,);


    Multiset_stopRunOnBadErrCode(this->addItemTimes(this, item, 1),);

    return MULTISET_E_OK;
}

static MultisetErrCode Multiset_addItemTimes(Multiset *this, MultisetItem item, size_t times) {
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);
    Multiset_autoprintErrAndStopRunIf(item == NULL, MULTISET_E_NULL_ARG,);


    if (times == 0) return MULTISET_E_OK;

    size_t index;
    Multiset_stopRunOnBadErrCode(Multiset_getItemIndex(item, this->_config, &index),);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    Multiset_stopRunOnBadErrCode(Multiset_convertAvlTreeErrCode(
        pAvlTree->addItemTimes(pAvlTree, (void *) item, times, &avlTreeNode)
    ),);

    this->itemsCount += times;
    if (avlTreeNode->count == times) ++this->uniqueItemsCount;

    return MULTISET_E_OK;
}


/*---------------------------------------------------- Remove Item ---------------------------------------------------*/

static MultisetErrCode Multiset_removeItem(Multiset *this, MultisetItem item) {
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);
    Multiset_autoprintErrAndStopRunIf(item == NULL, MULTISET_E_NULL_ARG,);


    size_t index;
    Multiset_stopRunOnBadErrCode(Multiset_getItemIndex(item, this->_config, &index),);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    Multiset_stopRunOnBadErrCode(Multiset_convertAvlTreeErrCode(
        pAvlTree->findItem(pAvlTree, (void *) item, &avlTreeNode)
    ),);

    if (avlTreeNode != NULL) {
        size_t itemCount = avlTreeNode->count;

        Multiset_stopRunOnBadErrCode(Multiset_convertAvlTreeErrCode(
            pAvlTree->removeItem(pAvlTree, (void *) item)
        ),);

        --this->itemsCount;
        if (itemCount == 1) --this->uniqueItemsCount;
    }

    return MULTISET_E_OK;
}

static MultisetErrCode Multiset_removeItemWithCopies(Multiset *this, MultisetItem item) {
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);
    Multiset_autoprintErrAndStopRunIf(item == NULL, MULTISET_E_NULL_ARG,);


    size_t index;
    Multiset_stopRunOnBadErrCode(Multiset_getItemIndex(item, this->_config, &index),);
    AvlTree *pAvlTree = &this->_data[index];

    AvlTreeNode *avlTreeNode;
    Multiset_stopRunOnBadErrCode(Multiset_convertAvlTreeErrCode(
        pAvlTree->findItem(pAvlTree, (void *) item, &avlTreeNode)
    ),);

    if (avlTreeNode != NULL) {
        size_t itemCount = avlTreeNode->count;

        Multiset_stopRunOnBadErrCode(Multiset_convertAvlTreeErrCode(
            pAvlTree->removeItemWithCopies(pAvlTree, (void *) item)
        ),);

        this->itemsCount -= itemCount;
        --this->uniqueItemsCount;
    }

    return MULTISET_E_OK;
}


/*----------------------------------------------------- Traverse -----------------------------------------------------*/

typedef struct {
    void *externalData;
    MultisetTraverserF multisetTraverserF;
} LayeredExternalData;

static void Multiset_avlTreeTraverserF(void *externalData, AvlTreeNode *node) {
    LayeredExternalData *layeredExternalData = externalData;
    layeredExternalData->multisetTraverserF(layeredExternalData->externalData, node->item, node->count);
}

static MultisetErrCode Multiset_traverse(Multiset *this, void *externalData, MultisetTraverserF traverser) {
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);
    Multiset_autoprintErrAndStopRunIf(traverser == NULL, MULTISET_E_NULL_ARG,);


    LayeredExternalData layeredExternalData = {externalData, traverser};

    const size_t multisetSize = this->_config.size;
    for (size_t i = 0; i < multisetSize; i++) {
        Multiset_stopRunOnBadErrCode(Multiset_convertAvlTreeErrCode(
            this->_data[i].traverse(&this->_data[i], &layeredExternalData, Multiset_avlTreeTraverserF)
        ),);
    }

    return MULTISET_E_OK;
}


/*------------------------------------------------------- Clear ------------------------------------------------------*/

static MultisetErrCode Multiset_clear(Multiset *this) {
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);


    const size_t multisetSize = this->_config.size;
    for (size_t i = 0; i < multisetSize; i++) {
        Multiset_stopRunOnBadErrCode(Multiset_convertAvlTreeErrCode(
            this->_data[i].clear(&this->_data[i])
        ),);
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
    Multiset_autoprintErrAndStopRunIf(this == NULL, MULTISET_E_NULL_THIS,);
    Multiset_autoprintErrAndStopRunIf(stats == NULL, MULTISET_E_NULL_ARG,);


    stats->config = this->_config;
    stats->itemsCount = this->itemsCount;
    stats->uniqueItemsCount = this->uniqueItemsCount;
    stats->maxCountWord = NULL;
    stats->maxCount = 0;

    Multiset_stopRunOnBadErrCode(this->traverse(this, stats, Multiset_getStatsMultisetTraverser),);

    return MULTISET_E_OK;
}
