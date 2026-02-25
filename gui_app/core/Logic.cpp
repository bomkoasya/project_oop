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
#include <thread>
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

void loadUserDataAsync(User& user, Database& db, std::function<void()> onComplete) {
    std::thread([&user, &db, onComplete]() {
        loadUserData(user, db);
        if (onComplete) {
            onComplete();
        }
    }).detach();
}

void saveUserDataAsync(User user, std::function<void()> onComplete) {

    std::thread([user, onComplete]() {
        saveUserData(user);
        if (onComplete) {
            onComplete();
        }
    }).detach();
}
