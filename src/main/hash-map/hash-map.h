#ifndef HASHMAP_171_HASH_MAP_H
#define HASHMAP_171_HASH_MAP_H

#include "hash-map-err.h"
#include "../avl-tree/avl-tree.h"

#include <stddef.h>


/*--------------------------------------------------- Hash Map Item --------------------------------------------------*/

typedef const char *String;

typedef struct {
    String key;
    size_t value;
} HashMapItem;

typedef void (*TraverserF)(HashMapItem item);


/*----------------------------------------------------- Hash Map -----------------------------------------------------*/

typedef struct hash_map_t HashMap;

struct hash_map_t {
    AvlTree *_data;

    size_t sectorCount;

    size_t itemCount;


    // Hash Map Interface

    HashMapErrCode (*getItem)(HashMap *this, String key, HashMapItem *destItem);

    HashMapErrCode (*putItem)(HashMap *this, HashMapItem item);

    HashMapErrCode (*removeItem)(HashMap *this, String key, HashMapItem *destItem);


    HashMapErrCode (*traverse)(HashMap *this, TraverserF traverser);

    HashMapErrCode (*clear)(HashMap *this);


    HashMapErrCode (*printStatistics)(HashMap *this);
};


/*-------------------------------------------- Memory Management Interface -------------------------------------------*/

HashMapErrCode HashMap_initHashMap(HashMap *map);

HashMapErrCode HashMap_eraseHashMap(HashMap *map);


HashMapErrCode HashMap_newHashMap(HashMap **pMap);

HashMapErrCode HashMap_freeHashMap(HashMap **pMap);


#endif // HASHMAP_171_HASH_MAP_H
