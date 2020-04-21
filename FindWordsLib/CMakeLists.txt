cmake_minimum_required(VERSION 3.16)
project(FindWordsLib)

set(HEADER_FILES
        Solver.h Trie.h
        bit_classes/BitSet.h bit_classes/BitMask.h
        service/LinearAllocator.h service/Timer.h)

set(SOURCE_FILES
        Solver.cpp Trie.cpp
        bit_classes/BitMask.cpp
        service/Timer.cpp)

add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} bit_classes service)