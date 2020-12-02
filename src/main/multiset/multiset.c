#include "multiset.h"

#include "multiset-impl.c"

#include <stdlib.h>
#include <string.h>


/*------------------------------------------------------- Utils ------------------------------------------------------*/

static inline bool Multiset_isConfigValid(MultisetConfig config) { return config.size > 0; }

static inline int Multiset_itemComparerForAvlTree(void *itemA, void *itemB) {
    return strcmp((MultisetItem) itemA, (MultisetItem) itemB);
}


/*--------------------------------- Memory Management Interface on the Stack and Heap --------------------------------*/

MultisetErrCode Multiset_initMultiset(Multiset *multiset, MultisetConfig config) {
    if (multiset == NULL) return MULTISET_E_NULL_ARG;
    if (!Multiset_isConfigValid(config)) return MULTISET_E_WRONG_CONFIG;

    AvlTreeErrCode avlTreeErrCode;


    *multiset = (Multiset) {0};


    multiset->_data = calloc(config.size, sizeof *multiset->_data);
    if (multiset->_data == NULL) {
        return MULTISET_E_MEM_ALLOC;
    }

    for (size_t i = 0; i < config.size; i++) {
        if ((avlTreeErrCode = AvlTree_initAvlTree(&multiset->_data[i], Multiset_itemComparerForAvlTree, free))) {
            return Multiset_convertAvlTreeErrCode(avlTreeErrCode);
        }
    }

    multiset->_config = config;

    // Add implementation
    multiset->hasItem = Multiset_hasItem;
    multiset->countItem = Multiset_countItem;
    multiset->addItem = Multiset_addItem;
    multiset->addItemTimes = Multiset_addItemTimes;
    multiset->removeItem = Multiset_removeItem;
    multiset->removeItemWithCopies = Multiset_removeItemWithCopies;

    multiset->traverse = Multiset_traverse;
    multiset->clear = Multiset_clear;
    multiset->getStatistics = Multiset_getStatistics;

    return MULTISET_E_OK;
}

MultisetErrCode Multiset_eraseMultiset(Multiset *multiset) {
    if (multiset == NULL) return MULTISET_E_NULL_ARG;

    MultisetErrCode errCode;


    if ((errCode = multiset->clear(multiset))) {
        return errCode;
    }

    free(multiset->_data);

    *multiset = (Multiset) {0};

    return MULTISET_E_OK;
}


/*-------------------------------------- Memory Management Interface on the Heap -------------------------------------*/

MultisetErrCode Multiset_allocMultiset(Multiset **pMultiset, MultisetConfig config) {
    if (pMultiset == NULL) return MULTISET_E_NULL_ARG;
    if (!Multiset_isConfigValid(config)) return MULTISET_E_WRONG_CONFIG;


    *pMultiset = malloc(sizeof **pMultiset);
    if (*pMultiset == NULL) {
        return MULTISET_E_MEM_ALLOC;
    }

    return Multiset_initMultiset(*pMultiset, config);
}

MultisetErrCode Multiset_freeMultiset(Multiset **pMultiset) {
    if (pMultiset == NULL) return MULTISET_E_NULL_ARG;

    MultisetErrCode errCode;


    if ((errCode = Multiset_eraseMultiset(*pMultiset))) {
        return errCode;
    }

    free(*pMultiset);
    *pMultiset = NULL;

    return MULTISET_E_OK;
}
