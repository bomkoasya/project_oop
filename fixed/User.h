#pragma once
#include <string>
#include <map>
#include <vector>
#include "Transaction.h"

/**
 * @file User.h
 * @brief Оголошення класу User, що представляє користувача програми.
 */

/**
 * @class User
 * @brief Основна модель даних, що представляє одного користувача.
 *
 * Цей клас зберігає всю інформацію, пов'язану з конкретним користувачем,
 * включаючи його особисті дані (ID, ім'я), налаштування (userPrefs)
 * та, найголовніше, повний вектор його фінансових транзакцій.
 */
class User {
public:
    std::string id;            ///< Унікальний ідентифікатор (логін) користувача.
    std::string name;          ///< Ім'я користувача для відображення в GUI.
    std::string email;         ///< Email користувача (може використовуватися для сповіщень або відновлення).
    std::map<std::string, std::string> userPrefs; ///< Карта налаштувань (напр., "theme": "dark").
    std::string defaultCurrency = "USD"; ///< Валюта за замовчуванням для нових транзакцій.

    std::vector<Transaction> transactions; ///< Повний список всіх транзакцій цього користувача.

    /**
     * @brief Шифрує конфіденційні дані користувача. (Наразі заглушка)
     * @param passphrase Секретна фраза-ключ для шифрування.
     */
    void encryptData(const std::string & /*passphrase*/);

    /**
     * @brief Створює резервну копію даних користувача. (Наразі заглушка)
     * @return Рядок, що ідентифікує створену резервну копію (напр., ID бекапу).
     */
    std::string backup() const;

    /**
     * @brief Відновлює дані користувача з резервної копії. (Наразі заглушка)
     * @param backupId Ідентифікатор бекапу, з якого потрібно відновитися.
     * @return true у разі успішного відновлення, інакше false.
     */
    bool restore(const std::string & /*backupId*/);
};



inline void User::encryptData(const std::string & /*passphrase*/) { /* placeholder */ }
inline std::string User::backup() const { return "backup-id-placeholder"; }
inline bool User::restore(const std::string & /*backupId*/) { return true; }
