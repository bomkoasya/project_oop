cmake_minimum_required(VERSION 3.14)
project(FinanceManagerProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG    release-1.12.1
)
FetchContent_MakeAvailable(googletest)

set(SOURCE_FILES
        Account.cpp
        CurrencyConverter.cpp
        Database.cpp
        Exporter.cpp
        ExponentialSmoothingStrategy.cpp
        Forecast.cpp
        ForecastModel.cpp
        Importer.cpp
        MovingAverageStrategy.cpp
        ReportGenerator.cpp
        Transaction.cpp
)

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

add_executable(FinanceManager main.cpp ${SOURCE_FILES})

enable_testing()
add_executable(run_tests ${TEST_FILES} ${SOURCE_FILES})
target_link_libraries(run_tests GTest::gtest_main)
include(GoogleTest)
    gtest_discover_tests(run_tests)