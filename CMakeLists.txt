cmake_minimum_required(VERSION 3.20)
project(finance_manager)

set(CMAKE_CXX_STANDARD 17)

# Додайте сюди всі ваші .cpp файли
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
)