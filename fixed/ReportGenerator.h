#pragma once
#include <vector>
#include <string>
#include <map>
#include "Transaction.h"

/**
 * @file ReportGenerator.h
 * @brief Оголошення класу ReportGenerator для обробки та експорту звітів.
 */

/**
 * @class ReportGenerator
 * @brief "Сервісний" клас для генерації фінансових звітів.
 *
 * Цей клас приймає вектор транзакцій та надає методи
 * для розрахунку статистики (загальна сума, за категоріями)
 * та експорту даних у різні формати (CSV, JSON).
 */
class ReportGenerator {
public:
    std::string reportTitle; ///< Заголовок, який буде використовуватися у згенерованих звітах.
    std::vector<Transaction> transactions; ///< Вектор транзакцій, на основі якого будується звіт.

    /**
     * @brief Конструктор ReportGenerator.
     * @param title Заголовок звіту, що встановлюється для `reportTitle`.
     */
    ReportGenerator(const std::string& title = "Finance Report")
        : reportTitle(title) {}

    /**
     * @brief Додає одну транзакцію до вектора `transactions`.
     * @param t Транзакція, яку потрібно додати.
     */
    void addTransaction(const Transaction& t);

    /**
     * @brief Розраховує загальну суму всіх транзакцій.
     * @return Загальна сума (double).
     */
    double calculateTotal() const;

    /**
     * @brief Розраховує загальну суму для конкретної категорії.
     * @param categoryId ID категорії, за якою фільтруються транзакції.
     * @return Загальна сума (double) для цієї категорії.
     */
    double calculateByCategory(const std::string& categoryId) const;

    /**
     * @brief Розраховує загальну суму для конкретного рахунку.
     * @param accountId ID рахунку, за яким фільтруються транзакції.
     * @return Загальна сума (double) для цього рахунку.
     */
    double calculateByAccount(const std::string& accountId) const;

    /**
     * @brief Експортує всі транзакції у файл формату CSV.
     * @param filePath Повний шлях до файлу (включно з .csv), куди буде збережено звіт.
     */
    void exportToCSV(const std::string& filePath) const;

    /**
     * @brief Експортує всі транзакції у файл формату JSON.
     * @param filePath Повний шлях до файлу (включно з .json), куди буде збережено звіт.
     */
    void exportToJSON(const std::string& filePath) const;

    /**
     * @brief Готує дані для кругової діаграми (Pie Chart).
     *
     * Групує транзакції за категоріями та підсумовує їх.
     *
     * @return `std::map`, де ключ (string) - це `categoryId`,
     * а значення (double) - це загальна сума для цієї категорії.
     */
    std::map<std::string, double> preparePieChartData() const;
};
