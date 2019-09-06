cmake_minimum_required(VERSION 3.15)
project (service)

add_library(${PROJECT_NAME} STATIC Timer.h Timer.cpp
                                   LinearAllocator.h LinearAllocator.cpp)