cmake_minimum_required(VERSION 3.16)
project(HashMap-171 C)

set(CMAKE_C_STANDARD 99)

add_executable(
    test
    src/test/main.c
    src/main/hash-functions/sha-1.c
    src/main/hash-functions/md5.c
    src/main/hash-functions/polynomial.c
    src/main/hash-map/hash-map.c src/main/hash-map/hash-map-err.c
    src/main/avl-tree/avl-tree.c src/main/avl-tree/avl-tree-err.c
)
