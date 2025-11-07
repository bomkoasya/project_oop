#pragma once

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

#include "Database.h"
#include "MovingAverageStrategy.h"

/**
 * @file ForecastWindow.h
 * @brief Оголошення діалогового вікна ForecastWindow для відображення фінансових прогнозів.
 */

/**
 * @class ForecastWindow
 * @brief Вікно для відображення фінансових прогнозів на основі наявних даних.
 *
 * Використовує стратегію ковзного середнього для розрахунку майбутніх витрат
 * та показує результат у текстовому полі.
 */
class ForecastWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класу ForecastWindow.
     * @param db Посилання на базу даних (лише для читання), з якої беруться дані для прогнозу.
     * @param parent Вказівник на батьківський віджет (зазвичай 0 або nullptr).
     */
    explicit ForecastWindow(const Database& db, QWidget *parent = nullptr);

private slots:
    /**
     * @brief Слот, що спрацьовує при натисканні кнопки "Розрахувати прогноз".
     *
     * Запитує у користувача кількість днів, виконує розрахунок та виводить результат у 'output'.
     */
    void onForecastClicked();

    /**
     * @brief Слот для закриття поточного діалогового вікна.
     */
    void onCloseClicked();

private:
    QTextEdit* output;     ///< Текстове поле для відображення результатів прогнозу.
    const Database& db;  ///< Посилання на базу даних для отримання історії транзакцій.
};
