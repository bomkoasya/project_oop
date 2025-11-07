#pragma once
#include "ForecastStrategy.h" // Включаємо базовий інтерфейс
#include <vector>

/**
 * @file MovingAverageStrategy.h
 * @brief Оголошення класу MovingAverageStrategy (Стратегія ковзного середнього).
 */

/**
 * @class MovingAverageStrategy
 * @brief Конкретна реалізація стратегії прогнозування, що використовує метод ковзного середнього.
 *
 * Цей клас успадковується від @c ForecastStrategy та реалізує його
 * віртуальний метод @c forecast.
 *
 * @see ForecastStrategy
 */
class MovingAverageStrategy : public ForecastStrategy {
public:
    /**
     * @brief Розраховує прогноз, використовуючи метод ковзного середнього.
     *
     * @param history Вектор минулих транзакцій для аналізу.
     * @param months Кількість останніх транзакцій (період),
     * які потрібно врахувати для розрахунку середнього.
     * @return Прогнозоване значення (double), розраховане як середнє арифметичне
     * 'months' останніх транзакцій.
     * @override Оголошує, що цей метод заміщує віртуальний метод базового класу.
     */
    double forecast(const std::vector<Transaction>& history, int months) override;
};
