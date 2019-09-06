cmake_minimum_required(VERSION 3.15)
project (FindWords)

set (CMAKE_BUILD_TYPE RELEASE)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

add_definitions(-Wall -O1)

add_subdirectory(bit_classes)
add_subdirectory(menu)
add_subdirectory(service)

if (${WIN32})
    add_executable(${PROJECT_NAME} FindWords.cpp Trie.h Trie.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE bit_classes menu service)
else()
    message(FATAL_ERROR "This program only for Windows. Sorry.")
endif()