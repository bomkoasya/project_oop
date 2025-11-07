#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * @file Transaction.h
 * @brief Оголошення класу Transaction, що представляє одну фінансову операцію.
 */

/**
 * @class Transaction
 * @brief Представляє одну фінансову операцію (дохід, витрата тощо).
 *
 * Це основна структура даних для зберігання всієї інформації,
 * пов'язаної з окремим фінансовим рухом, включаючи суму, дату,
 * категорію, теги та метадані.
 */
class Transaction {
public:
    // --- Основні дані ---
    std::string id;            ///< Унікальний ідентифікатор транзакції.
    double amount = 0.0;       ///< Сума транзакції (додатна - дохід, від'ємна - витрата).
    std::time_t date = 0;      ///< Дата та час транзакції (у форматі Unix timestamp).
    std::string description;   ///< Короткий опис, наданий користувачем.
    std::string categoryId;  ///< Ідентифікатор категорії (напр., "food", "rent").
    std::string accountId;     ///< Ідентифікатор рахунку, з якого/на який було здійснено платіж.

    // --- Валюта ---
    std::string currency = "USD"; ///< Код валюти
    double exchangeRate = 1.0;  ///< Обмінний курс відносно базової валюти (якщо не 1.0).

    // --- Метадані ---
    std::vector<std::string> tags; ///< Список тегів для фільтрації (напр., "work", "vacation").
    std::string recurringRule;   ///< Правило повторення (напр., "monthly:5" - щомісяця 5-го числа).
    std::string importedSource;  ///< Джерело імпорту (напр., "CSV:monobank.csv").
    std::string note;            ///< Додаткова розширена примітка.

    // --- Часові мітки ---
    std::time_t createdAt = 0; ///< Час створення запису (Unix timestamp).
    std::time_t updatedAt = 0; ///< Час останнього оновлення запису (Unix timestamp).


    /**
     * @brief Встановлює час створення транзакції. (Зараз - заглушка).
     */
    void addTransaction();

    /**
     * @brief Розділяє одну транзакцію на декілька менших.
     *
     * Корисно для випадків, коли один чек містить декілька категорій
     * (напр., у супермаркеті куплено "їжу" та "побутову хімію").
     *
     * @param parts Вектор пар, де кожна пара це {сума, categoryId} для нової транзакції.
     * @return Вектор нових об'єктів Transaction, створених на основі поточної.
     */
    std::vector<Transaction> splitTransaction(const std::vector<std::pair<double, std::string>>& parts);

    /**
     * @brief Додає тег до транзакції.
     * @param tag Рядок тегу, який потрібно додати до вектора `tags`.
     */
    void tagTransaction(const std::string& tag);

    /**
     * @brief Скасовує транзакцію (наразі заглушка).
     *
     * (Майбутня логіка може включати створення зворотної транзакції).
     */
    void undoTransaction();
};

// --- Серіалізація nlohmann/json ---

/**
 * @brief Серіалізує об'єкт Transaction у формат JSON.
 *
 * Автоматично викликається бібліотекою nlohmann/json, коли
 * ви робите `json j = transaction_object;`.
 *
 * @param j JSON-об'єкт (вихідний параметр), який буде заповнено.
 * @param t Об'єкт Transaction (вхідний, const) для серіалізації.
 */
inline void to_json(json& j, const Transaction& t) {
    j = json{
        {"id", t.id},
        {"amount", t.amount},
        {"date", t.date},
        {"description", t.description},
        {"categoryId", t.categoryId},
        {"accountId", t.accountId},
        {"currency", t.currency},
        {"exchangeRate", t.exchangeRate},
        {"tags", t.tags},
        {"recurringRule", t.recurringRule},
        {"importedSource", t.importedSource},
        {"note", t.note},
        {"createdAt", t.createdAt},
        {"updatedAt", t.updatedAt}
    };
}

/**
 * @brief Десеріалізує JSON в об'єкт Transaction.
 *
 * Автоматично викликається бібліотекою nlohmann/json, коли
 * ви робите `transaction_object = j.get<Transaction>();`.
 * Використовує `j.value()` для безпечного отримання даних з
 * наданням значень за замовчуванням.
 *
 * @param j JSON-об'єкт (вхідний, const) для читання.
 * @param t Об'єкт Transaction (вихідний параметр), який буде заповнено.
 */
inline void from_json(const json& j, Transaction& t) {
    // Використання .value("key", default) для безпечного парсингу
    t.id = j.value("id", "");
    t.amount = j.value("amount", 0.0);
    t.date = j.value("date", 0);
    t.description = j.value("description", "");
    t.categoryId = j.value("categoryId", "");
    t.accountId = j.value("accountId", "");
    t.currency = j.value("currency", "USD");
    t.exchangeRate = j.value("exchangeRate", 1.0);
    t.tags = j.value("tags", std::vector<std::string>{});
    t.recurringRule = j.value("recurringRule", "");
    t.importedSource = j.value("importedSource", "");
    t.note = j.value("note", "");
    t.createdAt = j.value("createdAt", 0);
    t.updatedAt = j.value("updatedAt", 0);
}

// --- Inline-реалізації ---

inline void Transaction::addTransaction() { createdAt = std::time(nullptr); }

inline std::vector<Transaction> Transaction::splitTransaction(const std::vector<std::pair<double, std::string>>& parts) {
    std::vector<Transaction> out;
    for (const auto &p : parts) {
        Transaction t = *this; // Копіюємо поточну транзакцію
        t.amount = p.first;    // Встановлюємо нову суму
        t.categoryId = p.second; // Встановлюємо нову категорію
        out.push_back(t);
    }
    return out;
}

inline void Transaction::tagTransaction(const std::string& tag) { tags.push_back(tag); }

inline void Transaction::undoTransaction() { /* placeholder */ }
