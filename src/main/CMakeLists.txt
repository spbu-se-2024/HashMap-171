add_executable(
    run_app
    main.c
    multiset/multiset.c multiset/multiset-err.c
    avl-tree/avl-tree.c avl-tree/avl-tree-err.c
    hash-functions/sha-1.c hash-functions/md5.c hash-functions/polynomial.c
)