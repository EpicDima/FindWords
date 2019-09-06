cmake_minimum_required(VERSION 3.15)
project (bit_classes)

add_library(${PROJECT_NAME} STATIC BitSet.h BitSet.cpp
                                   BitMask.h BitMask.cpp)