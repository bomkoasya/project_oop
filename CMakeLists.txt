cmake_minimum_required(VERSION 3.30)
project(programming_3sem)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(programming_3sem
        main.cpp
        Transaction.cpp
        Account.cpp
        CurrencyConverter.cpp
        Database.cpp
        Importer.cpp
        Exporte.cpp
        ReportGenerator.cpp
        Forecast.cpp
        ForecastModel.cpp
)
