// main.cpp
#include <iostream>
#include <memory>
#include "User.h"
#include "Account.h"
#include "Category.h"
#include "Budget.h"
#include "Transaction.h"
#include "CurrencyConverter.h"
#include "Database.h"
#include "Importer.h"
#include "Exporter.h"
#include "ForecastModel.h"
#include "ReportGenerator.h"

int main() {
    std::cout << "PFM demo (C++)\n";

    // Create user
    User u;
    u.id = "u1";
    u.name = "Alice";
    u.userPrefs["locale"] = "en-US";

    // Accounts
    CheckingAccount chk;
    chk.id = "acc:100";
    chk.name = "Checking";
    chk.balance = 1250.50;
    chk.currency = u.defaultCurrency;

    SavingsAccount sav;
    sav.id = "acc:200";
    sav.name = "Savings";
    sav.balance = 3000.0;
    sav.currency = u.defaultCurrency;

    // Category and budget
    Category groceries;
    groceries.id = "10";
    groceries.name = "Groceries";

    Budget b;
    b.id = "b1";
    b.userId = u.id;
    b.categoryId = groceries.id;
    b.amount = 400.0;
    b.period = "monthly";

    // Transaction sample
    Transaction tx;
    tx.id = "tx1";
    tx.amount = 54.23;
    tx.description = "Supermarket";
    tx.categoryId = groceries.id;
    tx.accountId = chk.id;
    tx.currency = "USD";

    // Database and persistence
    Database db;
    db.dbName = "memory";
    db.persistTransaction(tx);

    // Export sample
    CSVExporter csvExp("out.csv");
    csvExp.exportData({tx});


    // Report
    ReportGenerator rg;
    rg.transactions = { tx };
    std::cout << rg.generateSummary() << "\n";
    auto pie = rg.preparePieChartData();
    for (const auto &p : pie) {
        std::cout << "Pie slice: " << p.first << " -> " << p.second << "\n";
    }

    // Forecast
    ForecastModel fm;
    double forecast = fm.forecastNextMonth({ tx });
    std::cout << "Naive forecast next month: " << forecast << "\n";

    std::cout << "PFM demo finished.\n";
    return 0;
}
