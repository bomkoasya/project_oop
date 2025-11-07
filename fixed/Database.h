#pragma once
#include <string>
#include <vector>
#include <map>
#include "Transaction.h"

/**
 * @file Database.h
 * @brief Оголошення класу Database, що діє як in-memory сховище транзакцій.
 */

/**
 * @class Database
 * @brief Простий клас бази даних, що працює в оперативній пам'яті.
 *
 * Цей клас служить "кешем" або тимчасовим сховищем для транзакцій
 * програми, використовуючи `std::map` (`txStore`) для швидкого доступу
 * за ID. Він також надає інтерфейс для майбутніх операцій,
 * таких як бекап, відновлення та шифрування.
 *
 * @note Наразі більшість функцій є заглушками (placeholders).
 */
class Database {
public:
    std::string dbName; ///< Назва бази даних (напр., "user_profile").
    std::string dbPath; ///< Шлях до файлу бази даних (якщо вона зберігається на диску).
    std::map<std::string, Transaction> txStore; ///< In-memory сховище транзакцій (ID -> Об'єкт).

    /**
     * @brief Зберігає або оновлює транзакцію у `txStore`.
     * @param tx Об'єкт Transaction, який потрібно зберегти.
     * @return true, якщо збереження успішне.
     */
    bool persistTransaction(const Transaction &tx);

    /**
     * @brief Зберігає новий акаунт. (Наразі заглушка)
     * @param accountId Ідентифікатор акаунту для збереження.
     * @return true в разі успіху.
     */
    bool persistAccount(const std::string & /*accountId*/);

    /**
     * @brief Створює резервну копію бази даних. (Наразі заглушка)
     * @param targetPath Шлях, куди зберегти файл бекапу.
     * @return true в разі успіху.
     */
    bool backup(const std::string &targetPath);

    /**
     * @brief Відновлює базу даних з резервної копії. (Наразі заглушка)
     * @param sourcePath Шлях до файлу бекапу для відновлення.
     * @return true в разі успіху.
     */
    bool restore(const std::string &sourcePath);

    /**
     * @brief Шифрує файл бази даних. (Наразі заглушка)
     * @param passphrase Секретна фраза-ключ для шифрування.
     * @return true в разі успіху.
     */
    bool encryptData(const std::string & /*passphrase*/);

    /**
     * @brief Запитує всі транзакції зі сховища.
     * @return Вектор, що містить копії всіх транзакцій з `txStore`.
     */
    std::vector<Transaction> queryTransactions() const;
};

// --- Inline-реалізації ---

/**
 * @brief Реалізація: Додає транзакцію до `std::map` txStore.
 */
inline bool Database::persistTransaction(const Transaction &tx) {
    txStore[tx.id] = tx; // Вставить або оновить
    return true;
}

// --- Заглушки ---
inline bool Database::persistAccount(const std::string & /*accountId*/) { return true; }
inline bool Database::backup(const std::string & /*targetPath*/) { return true; }
inline bool Database::restore(const std::string & /*sourcePath*/) { return true; }
inline bool Database::encryptData(const std::string & /*passphrase*/) { return true; }

/**
 * @brief Реалізація: Копіює всі значення (транзакції) з map у вектор.
 */
inline std::vector<Transaction> Database::queryTransactions() const {
    std::vector<Transaction> out;
    out.reserve(txStore.size()); // Оптимізація: резервуємо пам'ять
    for (const auto &kv : txStore) {
        out.push_back(kv.second);
    }
    return out;
}
