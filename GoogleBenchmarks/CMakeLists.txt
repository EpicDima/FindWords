cmake_minimum_required(VERSION 3.16)
project(GoogleBenchmarks)

add_definitions(-lbenchmark -pthread)

add_executable(${PROJECT_NAME} test.cpp)

target_link_libraries(${PROJECT_NAME} FindWordsLib)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/FindWordsLib)

add_subdirectory(benchmark)
target_link_libraries(${PROJECT_NAME} benchmark::benchmark)