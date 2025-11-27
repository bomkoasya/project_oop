/**
 * @file Logic.cpp
 * @brief Реалізація основних функцій бізнес-логіки програми.
 *
 * Цей файл містить реалізації для завантаження/збереження даних користувача
 * (використовуючи nlohmann/json) та обробники для різних
 * консольних (CLI) операцій, таких як додавання транзакцій, звіти тощо.
 */

#include "Logic.h"
#include <iostream>
#include <iomanip> // Для setw, setprecision
#include <fstream>
#include <string>
#include <vector>
#include <limits> // Для numeric_limits
#include "Transaction.h"
#include "ReportGenerator.h"
#include "Importer.h"
#include "Exporter.h"
#include "CurrencyConverter.h"
#include "MovingAverageStrategy.h"
#include "Budget.h"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

/**
 * @brief Завантажує дані користувача з JSON-файлу.
 *
 * Шукає файл з іменем `<user.id>.json`. Якщо файл не знайдено,
 * виводить повідомлення про створення нового профілю.
 * Якщо знайдено, парсить JSON та десеріалізує його в об'єкт User
 * та заповнює об'єкт Database.
 *
 * @param user Об'єкт користувача (вихідний параметр), який буде заповнено.
 * @param db Об'єкт бази даних, який буде заповнено транзакціями.
 */
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
            tx_json.get_to(t);
            user.transactions.push_back(t);
            db.persistTransaction(t);
        }
    }

    cout << "User data loaded for " << user.id << "." << endl;
}

/**
 * @brief Зберігає дані користувача у JSON-файл.
 *
 * Серіалізує об'єкт User (включно з усіма транзакціями) у формат JSON
 * та зберігає його у файл з іменем `<user.id>.json`.
 *
 * @param user Об'єкт користувача (вхідний, const) для збереження.
 */
void saveUserData(const User& user) {
    json j;
    j["id"] = user.id;
    j["name"] = user.name;
    j["email"] = user.email;
    j["defaultCurrency"] = user.defaultCurrency;

    j["transactions"] = json::array();
    for (const auto& t : user.transactions) {
        json tx_json = t;
        j["transactions"].push_back(tx_json);
    }

    ofstream file(user.id + ".json");
    file << j.dump(4);
    cout << "User data saved to " << user.id << ".json" << endl;
}

/**
 * @brief Обробляє додавання транзакції через консольний інтерфейс (CLI).
 *
 * Запитує у користувача (через `cin`) ID, суму, опис та категорію.
 * Встановлює поточну дату та валюту за замовчуванням.
 * Додає транзакцію до `user` та `db`.
 *
 * @param user Об'єкт користувача, до якого додається транзакція.
 * @param db База даних, в яку зберігається транзакція.
 */
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

/**
 * @brief Відображає всі транзакції користувача у консолі.
 *
 * Форматує вивід у вигляді акуратної таблиці, використовуючи
 * `setw` (встановлення ширини) та `setprecision` (точність).
 *
 * @param user Користувач (const), чиї транзакції будуть відображені.
 */
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

/**
 * @brief Обробляє меню звітів у консольному режимі.
 *
 * Надає користувачу вибір: згенерувати повний звіт (в консоль),
 * експортувати у CSV або експортувати у JSON.
 * Використовує `ReportGenerator` для виконання цих дій.
 *
 * @param user Користувач (const), для якого генеруються звіти.
 */
void handleReports(const User& user) {
    if (user.transactions.empty()) {
        cout << "No transactions to generate a report." << endl;
        return;
    }

    ReportGenerator rg("User Finance Report");
    rg.transactions = user.transactions;

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

/**
 * @brief Обробляє меню імпорту/експорту даних у консольному режимі.
 *
 * Надає вибір: імпортувати з CSV або експортувати у CSV.
 * Використовує `CSVImporter` та `CSVExporter` для виконання операцій.
 * При імпорті додає транзакції до `user` та `db`.
 *
 * @param user Об'єкт користувача для імпорту/експорту.
 * @param db База даних, куди зберігаються імпортовані транзакції.
 */
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
        Result<vector<Transaction>> import_result = importer.import();

        if (import_result.is_success()) {
            vector<Transaction> imported = import_result.get_value();
            user.transactions.insert(user.transactions.end(), imported.begin(), imported.end());
            for(const auto& t : imported) {
                db.persistTransaction(t);
            }
            cout << imported.size() << " transactions imported successfully!" << endl;
        } else {
            cout << "Import failed: " << import_result.get_error() << endl;
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

/**
 * @brief Обробляє логіку прогнозування у консольному режимі.
 *
 * Запитує у користувача кількість транзакцій для аналізу (період).
 * Використовує `MovingAverageStrategy` для розрахунку прогнозу
 * та виводить результат у консоль.
 *
 * @param db База даних (const), з якої беруться дані для прогнозу.
 */
void handleForecasting(const Database& db) {
    if (db.txStore.empty()) {
        cout << "Not enough data to make a forecast." << endl;
        return;
    }

    MovingAverageStrategy strategy;
    vector<Transaction> history = db.queryTransactions();
    int months; // кількість транзакцій
    cout << "Enter number of recent transactions to consider for forecast: ";
    cin >> months;

    double prediction = strategy.forecast(history, months);

    cout << "\n--- Forecast ---" << endl;
    cout << "Based on the last " << months << " transactions, the predicted next amount is: "
         << fixed << setprecision(2) << prediction << endl;
}
