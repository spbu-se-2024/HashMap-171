add_executable(
    run_tests

    main.c

    ../../cutest-1.5/CuTest.c

    ../main/multiset/multiset.c ../main/multiset/multiset-err.c
    ../main/avl-tree/avl-tree.c ../main/avl-tree/avl-tree-err.c
    ../main/hash-functions/hash-function-err.c
    ../main/hash-functions/sha-1.c ../main/hash-functions/md5.c ../main/hash-functions/polynomial.c

    test-app.c
    test-multiset.c
    test-avl-tree.c
    test-sha-1.c
    test-md5.c
    test-polynomial.c
)
