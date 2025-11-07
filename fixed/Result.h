#pragma once

#include <string>
#include <stdexcept> // Для std::runtime_error
#include <utility>   // Для std::move

/**
 * @file Result.h
 * @brief Оголошення шаблонного класу Result<T> для обробки успішних/невдалих операцій.
 */

/**
 * @class Result
 * @brief Шаблонний клас, що інкапсулює результат операції, яка може бути успішною (з даними) або невдалою (з помилкою).
 *
 * Це "monad-like" клас, схожий на `std::optional` або `std::expected`.
 * Він допомагає уникнути використання винятків для обробки
 * очікуваних помилок (напр., "файл не знайдено" при імпорті).
 *
 * @tparam T Тип значення, яке повертається у разі успіху.
 *
 * @note Створення об'єктів відбувається через статичні
 * фабричні методи `Result<T>::success(value)` та `Result<T>::error(message)`.
 */
template <typename T>
class Result {
private:
    bool m_is_success;  ///< Прапорець, що показує, чи успішна операція.
    T m_value;          ///< Значення (payload) у разі успіху.
    std::string m_error; ///< Рядок з описом помилки у разі невдачі.

    /**
     * @brief Приватний конструктор.
     *
     * Використовується виключно фабричними методами `success()` та `error()`.
     * @param success Чи успішний результат.
     * @param value Значення для збереження (переміщується).
     * @param error Повідомлення про помилку (переміщується).
     */
    Result(bool success, T value, std::string error)
        : m_is_success(success), m_value(std::move(value)), m_error(std::move(error)) {}

public:
    /**
     * @brief Статичний фабричний метод для створення успішного результату.
     * @param value Значення, яке потрібно "загорнути" у Result.
     * @return Об'єкт Result з `is_success() == true`.
     */
    static Result<T> success(T value) {
        return Result(true, std::move(value), "");
    }

    /**
     * @brief Статичний фабричний метод для створення невдалого результату (помилки).
     * @param error_message Опис помилки, що сталася.
     * @return Об'єкт Result з `is_success() == false` та значенням T за замовчуванням.
     */
    static Result<T> error(const std::string& error_message) {
        return Result(false, T{}, error_message); // T{} створює значення за замовчуванням
    }

    /**
     * @brief Перевіряє, чи є результат успішним.
     * @return true, якщо операція була успішною, інакше false.
     */
    bool is_success() const {
        return m_is_success;
    }

    /**
     * @brief Отримує значення з успішного результату.
     *
     * @warning **Кидає виняток `std::runtime_error`**, якщо викликати
     * цей метод для невдалого результату (коли `is_success() == false`).
     * Завжди перевіряйте `is_success()` перед викликом `get_value()`.
     *
     * @return Константне посилання на збережене значення.
     * @throw std::runtime_error Якщо результат є помилкою.
     */
    const T& get_value() const {
        if (!m_is_success) {
            throw std::runtime_error("Attempted to get value from a failed Result: " + m_error);
        }
        return m_value;
    }

    /**
     * @brief Отримує повідомлення про помилку з невдалого результату.
     *
     * @return Константне посилання на рядок з описом помилки.
     * (Повертає порожній рядок, якщо результат успішний).
     */
    const std::string& get_error() const {
        return m_error;
    }
};
