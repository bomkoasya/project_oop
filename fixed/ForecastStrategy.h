#pragma once
#include <vector>
#include "Transaction.h"

/**
 * @file ForecastStrategy.h
 * @brief Оголошення абстрактного базового класу (інтерфейсу) ForecastStrategy.
 */

/**
 * @class ForecastStrategy
 * @brief Абстрактний базовий клас (інтерфейс) для стратегій прогнозування.
 *
 * Визначає загальний "контракт", якому повинні слідувати всі
 * конкретні реалізації (напр., @c MovingAverageStrategy).
 * Це дозволяє легко замінювати одну стратегію прогнозування іншою
 * (Патерн "Стратегія").
 *
 * @see MovingAverageStrategy
 */
class ForecastStrategy {
public:
    /**
     * @brief Віртуальний деструктор.
     *
     * Необхідний для коректного видалення об'єктів похідних класів
     * (напр., @c MovingAverageStrategy) через вказівник на базовий клас
     * (@c ForecastStrategy*).
     */
    virtual ~ForecastStrategy() {} // Використання {} замість = default

    /**
     * @brief Чисто віртуальний (pure virtual) метод для розрахунку прогнозу.
     *
     * Похідні класи **зобов'язані** реалізувати цей метод.
     *
     * @param history Вектор минулих транзакцій, що використовується для аналізу.
     * @param months Параметр, що визначає, як саме робиться прогноз.
     * (Наприклад, у @c MovingAverageStrategy це *кількість* останніх транзакцій).
     * @return Прогнозоване значення (double).
     */
    virtual double forecast(const std::vector<Transaction>& history, int months) = 0;
};
