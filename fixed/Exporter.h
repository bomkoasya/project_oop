#pragma once
#include <vector>
#include <string>
#include "Transaction.h"

/**
 * @file Exporter.h
 * @brief Оголошення класів-експортерів (напр., CSVExporter, ExcelExporter).
 * @note Цей файл, ймовірно, має називатися "Exporter.h" або "Exporters.h".
 */

// --- CSV Exporter ---

/**
 * @class CSVExporter
 * @brief Клас, що відповідає за експорт вектора транзакцій у CSV файл.
 */
class CSVExporter {
public:
    /**
     * @brief Конструктор CSVExporter.
     * @param filePath Повний шлях до файлу (включно з .csv), куди буде збережено дані.
     */
    CSVExporter(const std::string& filePath);

    /**
     * @brief Виконує процес експорту.
     *
     * Відкриває файл за шляхом `filePath`, записує рядок заголовків
     * (напр., "ID", "Amount", "Category") та ітерує по вектору
     * транзакцій, записуючи кожну у новому рядку.
     *
     * @param transactions Вектор транзакцій для експорту.
     * @return true у разі успішного запису файлу, інакше false.
     */
    bool exportData(const std::vector<Transaction>& transactions);

private:
    std::string filePath; ///< Шлях до цільового CSV файлу.
};

// --- Excel Exporter ---

/**
 * @class ExcelExporter
 * @brief Клас, що відповідає за експорт вектора транзакцій у Excel (.xlsx) файл.
 * @note Наразі, ймовірно, є заглушкою. Для реалізації потрібна
 * стороння бібліотека (напр., OpenXLSX, libxlsxwriter).
 */
class ExcelExporter {
public:
    /**
     * @brief Конструктор ExcelExporter.
     * @param filePath Повний шлях до файлу (включно з .xlsx), куди буде збережено дані.
     */
    ExcelExporter(const std::string& filePath);

    /**
     * @brief Виконує процес експорту в Excel.
     * @param transactions Вектор транзакцій для експорту.
     * @return true у разі успішного запису файлу, інакше false.
     */
    bool exportData(const std::vector<Transaction>& transactions);

private:
    std::string filePath; ///< Шлях до цільового .xlsx файлу.
};
