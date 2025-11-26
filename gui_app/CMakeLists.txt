cmake_minimum_required(VERSION 3.16)
project(GUIApp LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(GUIApp
    main.cpp
    core/Logic.cpp
    gui/MainWindow.cpp
    gui/ReportsWindow.cpp
    gui/TransactionsWindow.cpp
    gui/DataWindow.cpp
    gui/ForecastWindow.cpp
    gui/ShowTransactionsWindow.cpp
    resources.qrc
)
target_include_directories(GUIApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GUIApp PRIVATE core)
target_sources(GUIApp PRIVATE
    core/Logic.h
    gui/MainWindow.h
    gui/ReportsWindow.h
    gui/TransactionsWindow.h
    gui/DataWindow.h
    gui/ForecastWindow.h
    gui/ShowTransactionsWindow.h
)

target_link_libraries(GUIApp PRIVATE
    Qt6::Widgets
    fixed_logic
)
set(TS_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/translations/MyFinanceApp_uk.ts"
    "${CMAKE_CURRENT_SOURCE_DIR}/translations/MyFinanceApp_en.ts"
)
# Generate .qm files in source directory so resources.qrc can find them
qt6_add_translations(GUIApp SOURCES ${TS_FILES} OUTPUT_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/translations")

FILE(GLOB_RECURSE TR_SOURCES_CORE
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/*.h"
)
FILE(GLOB_RECURSE TR_SOURCES_GUI
    "${CMAKE_CURRENT_SOURCE_DIR}/gui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/gui/*.h"
)

add_custom_target(update_translations
    COMMAND ${Qt6_LUPDATE_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${TR_SOURCES_CORE}
        ${TR_SOURCES_GUI}
        -ts ${TS_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Updating translation source (.ts) files..."
)
