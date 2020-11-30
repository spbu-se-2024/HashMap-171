#include "multiset.h"

#include "multiset-impl.c"

#include <stdlib.h>


/*------------------------------------------------------- Utils ------------------------------------------------------*/

// AvlTreeItemCompF
static int Multiset_itemComparer(void *itemA, void *itemB) { return strcmp((String) itemA, (String) itemB); }


/*--------------------------------- Memory Management Interface on the Stack and Heap --------------------------------*/

MultisetErrCode Multiset_initMultiset(Multiset *multiset, MultisetConfig config) {
    if (multiset == NULL) return MULTISET_E_NULL_ARG;

    *multiset = (Multiset) {0};


    multiset->_data = malloc(config.size * sizeof(AvlTree));
    if (multiset->_data == NULL) return MULTISET_E_MEM_ALLOC;

    for (size_t i = 0; i < config.size; i++) {
        AvlTree_initAvlTree(&multiset->_data[i], Multiset_itemComparer, free);
    }

    multiset->_config = config;
    multiset->uniqueItemsCount = 0;
    multiset->itemsCount = 0;

    // Add implementation
    // TODO : Implement interface
    multiset->hasItem = Multiset_hasItem;
    multiset->countItem = Multiset_countItem;
    multiset->addItem = Multiset_addItem;
    // multiset->addItemTimes = Multiset_addItemTimes;
    multiset->removeItem = Multiset_removeItem;
    // multiset->removeItemWithCopies = Multiset_removeItemWithCopies;
    multiset->traverse = Multiset_traverse;
    multiset->clear = Multiset_clear;
    // multiset->printStatistics = Multiset_printStatistics;

    return MULTISET_E_OK;
}

MultisetErrCode Multiset_eraseMultiset(Multiset *multiset) {
    if (multiset == NULL) return MULTISET_E_NULL_ARG;

    multiset->clear(multiset);
    free(multiset->_data);

    *multiset = (Multiset) {0};

    return MULTISET_E_OK;
}


/*-------------------------------------- Memory Management Interface on the Heap -------------------------------------*/

MultisetErrCode Multiset_allocMultiset(Multiset **pMultiset, MultisetConfig config) {
    if (pMultiset == NULL) return MULTISET_E_NULL_ARG;


    *pMultiset = malloc(sizeof(Multiset));
    if (*pMultiset == NULL) return MULTISET_E_MEM_ALLOC;

    return Multiset_initMultiset(*pMultiset, config);
}

MultisetErrCode Multiset_freeMultiset(Multiset **pMultiset) {
    if (pMultiset == NULL) return MULTISET_E_NULL_ARG;


    Multiset_eraseMultiset(*pMultiset);

    free(*pMultiset);

    *pMultiset = NULL;

    return MULTISET_E_OK;
}
