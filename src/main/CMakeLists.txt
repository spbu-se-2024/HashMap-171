add_executable(
    run_app
    main.c
    hash-map/hash-map.c hash-map/hash-map-err.c
    avl-tree/avl-tree.c avl-tree/avl-tree-err.c
    hash-functions/sha-1.c hash-functions/md5.c
)