#ifndef HASHMAP_171_MULTISET_H
#define HASHMAP_171_MULTISET_H

#include "multiset-err.h"
#include "../avl-tree/avl-tree.h"

#include <stddef.h>


/*--------------------------------------------------- Multiset Item --------------------------------------------------*/

typedef const char *String;

typedef struct {
    String key;
    size_t value;
} MultisetItem;

typedef void (*TraverserF)(MultisetItem item);


/*----------------------------------------------------- Multiset -----------------------------------------------------*/

typedef struct multiset_t Multiset;

struct multiset_t {
    AvlTree *_data;

    size_t sectorCount;

    size_t itemCount;


    // Multiset Interface

    MultisetErrCode (*getItem)(Multiset *this, String key, MultisetItem *destItem);

    MultisetErrCode (*putItem)(Multiset *this, MultisetItem item);

    MultisetErrCode (*removeItem)(Multiset *this, String key, MultisetItem *destItem);


    MultisetErrCode (*traverse)(Multiset *this, TraverserF traverser);

    MultisetErrCode (*clear)(Multiset *this);


    MultisetErrCode (*printStatistics)(Multiset *this);
};


/*-------------------------------------------- Memory Management Interface -------------------------------------------*/

MultisetErrCode Multiset_initMultiset(Multiset *multiset);

MultisetErrCode Multiset_eraseMultiset(Multiset *multiset);


MultisetErrCode Multiset_newMultiset(Multiset **pMultiset);

MultisetErrCode Multiset_freeMultiset(Multiset **pMultiset);


#endif // HASHMAP_171_MULTISET_H
