cmake_minimum_required(VERSION 3.15)
project (FindWords)

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

#set (CMAKE_PREFIX_PATH "C:\\Qt\\5.14.0\\mingw73_64\\lib\\cmake")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

add_definitions(-Wall -O1)

if (${WIN32})
    find_package(Qt5Widgets REQUIRED)
    qt5_wrap_ui(QT_TEST_UI_HEADERS ui/MainWindow.ui)
    add_executable(${PROJECT_NAME} main.cpp
                                   Trie.h                    Trie.cpp
                                   bit_classes/BitMask.h     bit_classes/BitMask.cpp
                                   service/Timer.h           service/Timer.cpp
                                   Solver.cpp                Solver.h
                                   ui/MainWindow.h           ui/MainWindow.cpp
                                   ${QT_TEST_UI_HEADERS})

    target_include_directories(${PROJECT_NAME} PRIVATE bit_classes menu service)
    target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
else()
    message(FATAL_ERROR "This program only for Windows. Sorry.")
endif()
