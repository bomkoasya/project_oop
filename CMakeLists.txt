cmake_minimum_required(VERSION 3.16)
project(MyFinanceApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_subdirectory(fixed)
add_subdirectory(gui_app)
