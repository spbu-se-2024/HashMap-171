add_executable(
    run_tests
    main.c
    ../../cutest-1.5/CuTest.c
    ../main/hash-map/hash-map.c ../main/hash-map/hash-map-err.c
    ../main/avl-tree/avl-tree.c ../main/avl-tree/avl-tree-err.c
    ../main/hash-functions/sha-1.c ../main/hash-functions/md5.c
)
