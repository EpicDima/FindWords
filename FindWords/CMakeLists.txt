cmake_minimum_required(VERSION 3.15)
project (FindWords)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

add_definitions(-Wall -O1)

if (${WIN32})
    add_executable(${PROJECT_NAME} FindWords.cpp 
                                   Trie.h                    Trie.cpp
                                   bit_classes/BitMask.h     bit_classes/BitMask.cpp
                                   menu/BaseMenuItem.h       menu/BaseMenuItem.cpp
                                   menu/MenuItem.h           menu/MenuItem.cpp
                                   menu/Menu.h               menu/Menu.cpp
                                   service/Timer.h           service/Timer.cpp
                                   service/Localizer.h       service/Localizer.cpp)
    target_include_directories(${PROJECT_NAME} PRIVATE bit_classes menu service)
else()
    message(FATAL_ERROR "This program only for Windows. Sorry.")
endif()