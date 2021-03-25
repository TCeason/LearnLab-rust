cmake_minimum_required(VERSION 3.17)
project(hashtable)

set(CMAKE_CXX_STANDARD 14)

add_executable(hashtable hashmap.cpp hashbtree.cpp hashtable.cpp
        main.cpp)
