cmake_minimum_required(VERSION 3.16)
project(GoogleTests)

add_executable(${PROJECT_NAME} test.cpp)

target_link_libraries(${PROJECT_NAME} FindWordsLib)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/FindWordsLib)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} gtest gtest_main)
