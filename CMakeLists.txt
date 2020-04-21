cmake_minimum_required(VERSION 3.16)
project(FindWords)

set (CMAKE_CXX_STANDARD 17)

add_definitions(-Wall -O0)

add_subdirectory(FindWordsLib)
add_subdirectory(GoogleTests)
add_subdirectory(GoogleBenchmarks)
add_subdirectory(ConsoleGUI)
add_subdirectory(QtGUI)