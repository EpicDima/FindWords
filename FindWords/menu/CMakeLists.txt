cmake_minimum_required(VERSION 3.15)
project (menu)

add_library(${PROJECT_NAME} STATIC BaseMenuItem.h BaseMenuItem.cpp
                                   MenuItem.h     MenuItem.cpp
                                   Menu.h         Menu.cpp)