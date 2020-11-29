#ifndef HASHMAP_171_MULTISET_H
#define HASHMAP_171_MULTISET_H

#include "multiset-err.h"
#include "../avl-tree/avl-tree.h"

#include <stdbool.h>
#include <stddef.h>


/*-------------------------------------------------- Multiset Config -------------------------------------------------*/

typedef enum {
    MULTISET_HASH_FUNC_LABEL_SHA_1,
    MULTISET_HASH_FUNC_LABEL_MD5,
    MULTISET_HASH_FUNC_LABEL_POLYNOMIAL,
} MultisetHashFuncLabel;

typedef struct {
    MultisetHashFuncLabel hashFuncLabel;
    size_t size;
} MultisetConfig;


/*--------------------------------------------------- Multiset Item --------------------------------------------------*/

typedef const char *String;

typedef void (*MultisetTraverserF)(String item, size_t count);


/*----------------------------------------------------- Multiset -----------------------------------------------------*/

typedef struct multiset_t Multiset;

struct multiset_t {
    AvlTree *_data;

    MultisetConfig _config;

    size_t uniqueItemsCount;

    size_t itemsCount;


    // Multiset Interface

    MultisetErrCode (*hasItem)(Multiset *this, String item, bool *hasItem);

    MultisetErrCode (*countItem)(Multiset *this, String item, size_t *itemCount);

    MultisetErrCode (*addItem)(Multiset *this, String item);

    MultisetErrCode (*addItemTimes)(Multiset *this, String item, size_t times);

    MultisetErrCode (*removeItem)(Multiset *this, String item);

    MultisetErrCode (*removeItemWithCopies)(Multiset *this, String item);


    MultisetErrCode (*traverse)(Multiset *this, MultisetTraverserF traverser);

    MultisetErrCode (*clear)(Multiset *this);


    MultisetErrCode (*printStatistics)(Multiset *this);
};


/*-------------------------------------------- Memory Management Interface -------------------------------------------*/

MultisetErrCode Multiset_initMultiset(Multiset *multiset, MultisetConfig config);

MultisetErrCode Multiset_eraseMultiset(Multiset *multiset);


MultisetErrCode Multiset_allocMultiset(Multiset **pMultiset, MultisetConfig config);

MultisetErrCode Multiset_freeMultiset(Multiset **pMultiset);


#endif // HASHMAP_171_MULTISET_H
