cmake_minimum_required(VERSION 3.14)
project(fixed_logic)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCE_FILES
        Account.cpp
        Account.h
        CurrencyConverter.cpp
        CurrencyConverter.h
        Database.cpp
        Database.h
        Exporter.cpp
        Exporter.h
        ExponentialSmoothingStrategy.cpp
        ExponentialSmoothingStrategy.h
        Forecast.cpp
        Forecast.h
        ForecastModel.cpp
        ForecastModel.h
        Importer.cpp
        Importer.h
        MovingAverageStrategy.cpp
        MovingAverageStrategy.h
        ReportGenerator.cpp
        ReportGenerator.h
        Transaction.cpp
        Transaction.h
        User.h
)

add_library(fixed_logic STATIC ${LIB_SOURCE_FILES})

target_include_directories(fixed_logic PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(fixed_logic PUBLIC cpr::cpr nlohmann_json::nlohmann_json)

option(BUILD_TESTING "Build unit tests" ON)
if (BUILD_TESTING)
    enable_testing()

    set(TEST_FILES
            tests/test_account.cpp
            tests/test_budget.cpp
            tests/test_category.cpp
            tests/test_currency_converter.cpp
            tests/test_database.cpp
            tests/test_exporter.cpp
            tests/test_forecast.cpp
            tests/test_forecast_strategy.cpp
            tests/test_main.cpp
            tests/test_report_generator.cpp
            tests/test_transaction.cpp
    )

    add_executable(run_tests ${TEST_FILES})
    
    target_link_libraries(run_tests PRIVATE fixed_logic GTest::gtest_main GTest::gmock_main)
    
    include(GoogleTest)
    gtest_discover_tests(run_tests)
endif()