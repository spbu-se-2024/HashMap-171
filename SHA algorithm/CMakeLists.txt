cmake_minimum_required(VERSION 3.16)
project(SHA_algorithm C)

set(CMAKE_C_STANDARD 99)

add_executable(SHA_algorithm src/example.c src/SHA.h src/SHA.c)