#pragma once
#include <string>
#include <vector>
#include <ctime> // Додано для parseDate
#include "Transaction.h"
#include "Result.h" // Припускаю, що цей файл у вас є

/**
 * @file Importer.h
 * @brief Оголошення абстрактного базового класу Importer та його конкретних реалізацій (CSVImporter).
 */

/**
 * @class Importer
 * @brief Абстрактний базовий клас (інтерфейс) для всіх імпортерів даних.
 *
 * Визначає загальний контракт, якому повинні слідувати всі класи,
 * що імпортують транзакції з будь-якого джерела (CSV, JSON, API банку тощо).
 */
class Importer {
public:
    std::string sourceName; ///< Назва джерела (напр., "Monobank API", "MyBank.csv").

    /**
     * @brief Віртуальний деструктор.
     *
     * Необхідний для коректного видалення об'єктів похідних класів
     * через вказівник на базовий клас.
     */
    virtual ~Importer() = default;

    /**
     * @brief Чисто віртуальний метод для виконання процесу імпорту.
     *
     * Похідні класи повинні реалізувати цей метод, щоб зчитати дані
     * зі свого джерела, розпарсити їх та повернути вектор транзакцій.
     *
     * @return Об'єкт `Result`, що містить або `std::vector<Transaction>`
     * у разі успіху, або рядок з описом помилки у разі невдачі.
     */
    virtual Result<std::vector<Transaction>> import() = 0;
};

// --- CSV Importer ---

/**
 * @class CSVImporter
 * @brief Конкретна реалізація імпортера, що читає транзакції з CSV файлу.
 *
 * Успадковується від @c Importer та реалізує його метод @c import.
 * @see Importer
 */
class CSVImporter : public Importer {
public:
    /**
     * @brief Конструктор CSVImporter.
     * @param filePath Шлях до CSV файлу, з якого буде відбуватися імпорт.
     */
    CSVImporter(const std::string& filePath);

    /**
     * @brief Виконує імпорт транзакцій з CSV файлу.
     *
     * Відкриває файл, читає його рядок за рядком, парсить кожну
     * транзакцію та повертає результат.
     *
     * @return Об'єкт `Result`, що містить вектор транзакцій або помилку.
     * @override
     */
    Result<std::vector<Transaction>> import() override;

private:
    /**
     * @brief Допоміжна функція для парсингу дати з рядка.
     *
     * Перетворює рядок дати (напр., "2025-11-07") у формат `std::time_t`.
     *
     * @param dateStr Рядок, що містить дату.
     * @return `std::time_t` (Unix timestamp).
     */
    std::time_t parseDate(const std::string& dateStr) const;

    std::string path; ///< Шлях до CSV файлу, що зберігається з конструктора.
};
