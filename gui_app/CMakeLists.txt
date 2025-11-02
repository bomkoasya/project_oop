project(GUIApp LANGUAGES CXX)

add_executable(GUIApp
    main.cpp
    core/Logic.cpp
    core/Logic.h
    gui/MainWindow.cpp
    gui/MainWindow.h
    gui/ReportsWindow.cpp
    gui/ReportsWindow.h
    gui/TransactionsWindow.cpp
    gui/TransactionsWindow.h
    gui/DataWindow.cpp
    gui/DataWindow.h
    gui/ForecastWindow.cpp
    gui/ForecastWindow.h
    gui/ForecastWindow.cpp
    gui/ShowTransactionsWindow.h
    gui/ShowTransactionsWindow.cpp
)

target_include_directories(GUIApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor
)

target_link_libraries(GUIApp PRIVATE
    fixed_logic
    Qt6::Widgets
)
