cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

set(BENCHMARK_FILES
        benchmark_forecast.cpp
        benchmark_database.cpp
        benchmark_report_generator.cpp
        benchmark_forecast_strategy.cpp
        benchmark_exporter.cpp
        benchmark_account.cpp
        benchmark_main.cpp
)

add_executable(finance_benchmarks ${BENCHMARK_FILES})
target_link_libraries(finance_benchmarks PRIVATE
        fixed_logic
        benchmark::benchmark
        benchmark::benchmark_main
)

target_include_directories(finance_benchmarks PRIVATE
        ${CMAKE_SOURCE_DIR}/fixed
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(finance_benchmarks PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        FOLDER "Benchmarks"
)


