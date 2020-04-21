cmake_minimum_required(VERSION 3.16)
project(FindWordsQt)

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

qt5_wrap_ui(QT_TEST_UI_HEADERS MainWindow.ui)
add_executable(${PROJECT_NAME}
        main.cpp
        MainWindow.h MainWindow.cpp
        ${QT_TEST_UI_HEADERS})

target_link_libraries(${PROJECT_NAME} FindWordsLib Qt5::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/FindWordsLib)