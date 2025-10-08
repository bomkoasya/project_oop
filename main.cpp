#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <numeric>
#include <limits>

#include "json.hpp"
#include "User.h"
#include "Transaction.h"
#include "Account.h"
#include "ReportGenerator.h"
#include "Importer.h"
#include "Exporter.h"
#include "CurrencyConverter.h"
#include "MovingAverageStrategy.h"
#include "Budget.h"
#include "Database.h"


using namespace std;
using json = nlohmann::json;


void from_json(const json& j, Transaction& t) {
    j.at("id").get_to(t.id);
    j.at("amount").get_to(t.amount);
    j.at("date").get_to(t.date);
    j.at("description").get_to(t.description);
    j.at("categoryId").get_to(t.categoryId);
    j.at("accountId").get_to(t.accountId);
    j.at("currency").get_to(t.currency);

    if (j.contains("tags")) {
        j.at("tags").get_to(t.tags);
    }
    if (j.contains("note")) {
        j.at("note").get_to(t.note);
    }
}

void to_json(json& j, const Transaction& t) {
    j = json{
        {"id", t.id},
        {"amount", t.amount},
        {"date", t.date},
        {"description", t.description},
        {"categoryId", t.categoryId},
        {"accountId", t.accountId},
        {"currency", t.currency},
        {"tags", t.tags},
        {"note", t.note}
    };
}


void loadUserData(User& user, Database& db) {
    ifstream file(user.id + ".json");
    if (!file.is_open()) {
        cout << "New user profile created for: " << user.id << endl;
        return;
    }

    json j;
    file >> j;

    j.at("id").get_to(user.id);
    j.at("name").get_to(user.name);
    j.at("email").get_to(user.email);
    j.at("defaultCurrency").get_to(user.defaultCurrency);

    if (j.contains("transactions")) {
        for (const auto& tx_json : j["transactions"]) {
            Transaction t;
            from_json(tx_json, t);
            user.transactions.push_back(t);
            db.persistTransaction(t);
        }
    }

    cout << "User data loaded for " << user.id << "." << endl;
}

void saveUserData(const User& user) {
    json j;
    j["id"] = user.id;
    j["name"] = user.name;
    j["email"] = user.email;
    j["defaultCurrency"] = user.defaultCurrency;
    j["transactions"] = json::array();

    for (const auto& t : user.transactions) {
        json tx_json;
        to_json(tx_json, t);
        j["transactions"].push_back(tx_json);
    }

    ofstream file(user.id + ".json");
    file << j.dump(4);
    cout << "User data saved to " << user.id << ".json" << endl;
}


void addTransaction(User& user, Database& db) {
    Transaction t;
    cout << "Enter transaction ID: ";
    cin >> t.id;
    cout << "Amount: ";
    cin >> t.amount;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Description: ";
    getline(cin, t.description);
    cout << "Category ID: ";
    cin >> t.categoryId;

    t.date = time(nullptr);
    t.currency = user.defaultCurrency;
    t.addTransaction();

    user.transactions.push_back(t);
    db.persistTransaction(t);
    cout << "Transaction added successfully!" << endl;
}

void showTransactions(const User& user) {
    if (user.transactions.empty()) {
        cout << "No transactions found.\n";
        return;
    }

    cout << "\n--- Transactions for user: " << user.name << " ---\n";
    cout << left << setw(10) << "ID" << setw(12) << "Amount"
         << setw(15) << "Category" << setw(30) << "Description" << endl;
    cout << string(67, '-') << endl;

    for (const auto& t : user.transactions) {
        cout << left << setw(10) << t.id
             << fixed << setprecision(2) << setw(12) << t.amount
             << setw(15) << t.categoryId
             << setw(30) << t.description << endl;
    }
}

void handleReports(const User& user) {
    if (user.transactions.empty()) {
        cout << "No transactions to generate a report." << endl;
        return;
    }

    ReportGenerator rg("User Finance Report");
    rg.transactions = user.transactions; // Копіюємо транзакції до генератора

    cout << "\n--- Report Menu ---\n";
    cout << "1. Full Report\n";
    cout << "2. Export to CSV\n";
    cout << "3. Export to JSON\n";
    cout << "Your choice: ";
    int choice;
    cin >> choice;

    string path;
    switch (choice) {
        case 1:
            cout << "\n--- Full Report ---\n";
            cout << "Total amount: " << rg.calculateTotal() << endl;
            break;
        case 2:
            cout << "Enter path for CSV export: ";
            cin >> path;
            rg.exportToCSV(path);
            cout << "Report exported to " << path << endl;
            break;
        case 3:
            cout << "Enter path for JSON export: ";
            cin >> path;
            rg.exportToJSON(path);
            cout << "Report exported to " << path << endl;
            break;
        default:
            cout << "Invalid choice.\n";
    }
}

void handleDataIO(User& user, Database& db) {
    cout << "\n--- Data I/O Menu ---\n";
    cout << "1. Import from CSV\n";
    cout << "2. Export to CSV\n";
    cout << "Your choice: ";
    int choice;
    cin >> choice;

    string path;
    if (choice == 1) {
        cout << "Enter CSV file path to import from: ";
        cin >> path;
        CSVImporter importer(path);
        vector<Transaction> imported = importer.import();
        if (!imported.empty()) {
            user.transactions.insert(user.transactions.end(), imported.begin(), imported.end());
            for(const auto& t : imported) {
                db.persistTransaction(t);
            }
            cout << imported.size() << " transactions imported successfully!" << endl;
        } else {
            cout << "No transactions were imported." << endl;
        }
    } else if (choice == 2) {
        if (user.transactions.empty()) {
            cout << "No transactions to export." << endl;
            return;
        }
        cout << "Enter CSV file path to export to: ";
        cin >> path;
        CSVExporter exporter(path);
        if (exporter.exportData(user.transactions)) {
            cout << "Transactions exported successfully to " << path << endl;
        } else {
            cout << "Failed to export transactions." << endl;
        }
    } else {
        cout << "Invalid choice.\n";
    }
}

void handleForecasting(const Database& db) {
    if (db.txStore.empty()) {
        cout << "Not enough data to make a forecast." << endl;
        return;
    }

    MovingAverageStrategy strategy;
    vector<Transaction> history = db.queryTransactions();
    int months;
    cout << "Enter number of recent transactions to consider for forecast: ";
    cin >> months;

    double prediction = strategy.forecast(history, months);

    cout << "\n--- Forecast ---" << endl;
    cout << "Based on the last " << months << " transactions, the predicted next amount is: "
         << fixed << setprecision(2) << prediction << endl;
}

int main() {
    cout << "=== Personal Finance Manager ===" << endl;
    User user;
    Database db;

    cout << "Enter your username (this will be your profile ID): ";
    cin >> user.id;
    user.name = user.id;

    loadUserData(user, db);

    int choice;
    do {
        cout << "\n============== Main Menu ==============\n";
        cout << "1. Add Transaction\n";
        cout << "2. Show All Transactions\n";
        cout << "3. Generate Reports\n";
        cout << "4. Data Import/Export\n";
        cout << "5. Forecast\n";
        cout << "6. Save and Exit\n";
        cout << "0. Exit Without Saving\n";
        cout << "=======================================\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addTransaction(user, db);
                break;
            case 2:
                showTransactions(user);
                break;
            case 3:
                handleReports(user);
                break;
            case 4:
                handleDataIO(user, db);
                break;
            case 5:
                handleForecasting(db);
                break;
            case 6:
                saveUserData(user);
                choice = 0;
                break;
            case 0:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}