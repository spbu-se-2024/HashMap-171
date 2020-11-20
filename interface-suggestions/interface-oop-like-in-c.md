# Interface (Azim)

## OOP-like in C


### Хотелось бы сделать OOP-like в C:

```c
int main(void) {
    HashMapErr status;

    HashMap map;

    status = HashMap_initMap(
        &map,
        (HashMapConfig){ .hashFunc = HASH_MAP_F_SHA1, .size = 1000 }
    );
    HashMap_panicIf(status);

    status = map.put(&map, (HashMapKeyValue) { .key = "Alisa", .value = 20 });
    HashMap_panicIf(status);

    status = map.put(&map, (HashMapKeyValue) { .key = "Bob", .value = 15 });
    HashMap_panicIf(status);

    status = map.printStatistics(&map, stdout);
    HashMap_panicIf(status);

    status = map.erase(&map);
    HashMap_panicIf(status);

    return EXIT_SUCCESS;
}
```


### Как этого достичь?

В header:

```c
    typedef struct {
        // fields
        // ...
        // fields

        // this - в C++ по сути this подставляется автаматически, а в C придется делать вручную
        HashMapErr (*put)(HashMap *this, ...);

        HashMapErr (*foo)(HashMap *this, ...);

        HashMapErr (*bar)(HashMap *this, ...);

    } HashMap;
```

В initMap(...):

```c
   // HashMap_put - pointer к статической функции, поэтому HashMap_put не будет виден снаружи файла реализации.
   map->put = HashMap_put;
```


### Чего мы добились:

- Чистый namespace (отсутвие засоренности такими функциями как *HashMap_put*, *HashMap_get*, ...)
- Читаемость
- Красивый вид
