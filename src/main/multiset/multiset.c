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
    Multiset_autoprintErrAndStopRunIf(multiset == NULL, MULTISET_E_NULL_ARG,);
    Multiset_autoprintErrAndStopRunIf(!Multiset_isConfigValid(config), MULTISET_E_WRONG_CONFIG,);


    *multiset = (Multiset) {0};


    multiset->_data = calloc(config.size, sizeof *multiset->_data);
    Multiset_autoprintErrAndStopRunIf(multiset->_data == NULL, MULTISET_E_MEM_ALLOC,);

    for (size_t i = 0; i < config.size; i++) {
        Multiset_stopRunOnBadErrCode(
            Multiset_convertAvlTreeErrCode(
                AvlTree_initAvlTree(&multiset->_data[i], Multiset_itemComparerForAvlTree, free)
            ),
            { free(multiset->_data); }
        );
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
    Multiset_autoprintErrAndStopRunIf(multiset == NULL, MULTISET_E_NULL_ARG,);


    Multiset_stopRunOnBadErrCode(multiset->clear(multiset),);

    free(multiset->_data);

    *multiset = (Multiset) {0};

    return MULTISET_E_OK;
}


/*-------------------------------------- Memory Management Interface on the Heap -------------------------------------*/

MultisetErrCode Multiset_allocMultiset(Multiset **pMultiset, MultisetConfig config) {
    Multiset_autoprintErrAndStopRunIf(pMultiset == NULL, MULTISET_E_NULL_ARG,);
    Multiset_autoprintErrAndStopRunIf(!Multiset_isConfigValid(config), MULTISET_E_WRONG_CONFIG,);


    *pMultiset = malloc(sizeof **pMultiset);
    Multiset_autoprintErrAndStopRunIf(*pMultiset == NULL, MULTISET_E_MEM_ALLOC,);

    Multiset_stopRunOnBadErrCode(Multiset_initMultiset(*pMultiset, config), {
        free(pMultiset);
    });

    return MULTISET_E_OK;
}

MultisetErrCode Multiset_freeMultiset(Multiset **pMultiset) {
    Multiset_autoprintErrAndStopRunIf(pMultiset == NULL, MULTISET_E_NULL_ARG,);


    Multiset_stopRunOnBadErrCode(Multiset_eraseMultiset(*pMultiset),);

    free(*pMultiset);
    *pMultiset = NULL;

    return MULTISET_E_OK;
}
