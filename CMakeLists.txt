cmake_minimum_required(VERSION 3.21)
project(SynonymSeekerQtGUI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/janiafernan/Qt/6.2.2/macos/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(SynonymSeekerQtGUI main.cpp Widget.cpp Widget.h Widget.ui Graph.h Graph.cpp)
target_link_libraries(SynonymSeekerQtGUI
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

