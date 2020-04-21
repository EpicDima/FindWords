cmake_minimum_required(VERSION 3.16)
project(FindWordsConsole)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} FindWordsLib)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/FindWordsLib)