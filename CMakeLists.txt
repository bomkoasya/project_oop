cmake_minimum_required(VERSION 3.20)
project(finance_manager_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(finance_manager
        main.cpp
        ReportGenerator.cpp
        Importer.cpp
        Exporter.cpp
        MovingAverageStrategy.cpp
        Forecast.cpp
        Transaction.cpp
        Account.cpp
        CurrencyConverter.cpp
        Database.cpp
        ForecastModel.cpp
        Result.h
)


enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)


add_executable(run_tests

        tests/test_main.cpp
        tests/test_transaction.cpp
        tests/test_account.cpp
        tests/test_report_generator.cpp
        tests/test_currency_converter.cpp
        tests/test_database.cpp

        Transaction.cpp
        Account.cpp
        ReportGenerator.cpp
        CurrencyConverter.cpp
        Database.cpp
        tests/test_budget.cpp
        tests/test_category.cpp
        tests/test_exporter.cpp
        tests/test_forecast.cpp
        tests/test_forecast_strategy.cpp
        tests/test_importer.cpp
)

target_link_libraries(
        run_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)