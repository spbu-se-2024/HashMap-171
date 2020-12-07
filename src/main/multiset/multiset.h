#ifndef HASHMAP_171_MULTISET_H
#define HASHMAP_171_MULTISET_H

#include "multiset-err.h"
#include "../avl-tree/avl-tree.h"

#include <stdbool.h>
#include <stddef.h>


/*-------------------------------------------------- Multiset Config -------------------------------------------------*/

typedef enum {
    MULTISET_HASH_FUNC_LABEL_MD5,
    MULTISET_HASH_FUNC_LABEL_POLYNOMIAL,
    MULTISET_HASH_FUNC_LABEL_SHA_1,
} MultisetHashFuncLabel;

typedef struct {
    MultisetHashFuncLabel hashFuncLabel;
    size_t size;
} MultisetConfig;


/*--------------------------------------------------- Multiset Item --------------------------------------------------*/

typedef const char *MultisetItem;

typedef void (*MultisetTraverserF)(void *externalData, MultisetItem item, size_t count);


/*------------------------------------------------ Multiset Statistics -----------------------------------------------*/

typedef struct {
    MultisetConfig config;
    size_t itemsCount;
    size_t uniqueItemsCount;
    const char *maxCountWord;
    size_t maxCount;
} MultisetStats;


/*----------------------------------------------------- Multiset -----------------------------------------------------*/

typedef struct multiset_t Multiset;

struct multiset_t {
    AvlTree *_data;

    MultisetConfig _config;

    size_t itemsCount;

    size_t uniqueItemsCount;


    // Multiset Interface

    MultisetErrCode (*hasItem)(Multiset *this, MultisetItem item, bool *hasItem);

    MultisetErrCode (*countItem)(Multiset *this, MultisetItem item, size_t *itemCount);

    MultisetErrCode (*addItem)(Multiset *this, MultisetItem item);

    MultisetErrCode (*addItemTimes)(Multiset *this, MultisetItem item, size_t times);

    MultisetErrCode (*removeItem)(Multiset *this, MultisetItem item);

    MultisetErrCode (*removeItemWithCopies)(Multiset *this, MultisetItem item);


    MultisetErrCode (*traverse)(Multiset *this, void *externalData, MultisetTraverserF traverser);

    MultisetErrCode (*clear)(Multiset *this);

    MultisetErrCode (*getStatistics)(Multiset *this, MultisetStats *multisetStats);
};


/*-------------------------------------------- Memory Management Interface -------------------------------------------*/

MultisetErrCode Multiset_initMultiset(Multiset *multiset, MultisetConfig config);

MultisetErrCode Multiset_eraseMultiset(Multiset *multiset);


MultisetErrCode Multiset_allocMultiset(Multiset **pMultiset, MultisetConfig config);

MultisetErrCode Multiset_freeMultiset(Multiset **pMultiset);


#endif // HASHMAP_171_MULTISET_H
