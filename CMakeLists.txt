cmake_minimum_required(VERSION 3.16)
project(HashMap-171 C)

set(CMAKE_C_STANDARD 99)

add_executable(
    test
    src/test/main.c
    src/main/hash-functions/sha-1.c
    src/main/libs/bst/bst.c src/main/libs/bst/bst_err.c
)
