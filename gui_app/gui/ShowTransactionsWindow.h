#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include "Database.h"
#include "CurrencyConverter.h"

/**
 * @file ShowTransactionsWindow.h
 * @brief Оголошення діалогового вікна ShowTransactionsWindow для відображення транзакцій.
 */

/**
 * @class ShowTransactionsWindow
 * @brief Вікно, що відображає список всіх транзакцій користувача у вигляді таблиці.
 *
 * Це вікно призначене лише для читання; воно завантажує дані з бази даних
 * і показує їх у @c QTableWidget.
 */
class ShowTransactionsWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класу ShowTransactionsWindow.
     * @param db Посилання на базу даних (лише для читання), з якої завантажуються транзакції.
     * @param parent Вказівник на батьківський віджет (зазвичай 0 або nullptr).
     */
    explicit ShowTransactionsWindow(const Database& db, QWidget *parent = nullptr);

private slots:
    /**
     * @brief Слот для оновлення курсу валют з API.
     */
    void onUpdateExchangeRates();

    /**
     * @brief Слот, що спрацьовує при зміні вибраної валюти.
     */
    void onCurrencyChanged();

private:
    /**
     * @brief Оновлює таблицю транзакцій з урахуванням вибраної валюти.
     */
    void refreshTable();

    QTableWidget *table; ///< Таблиця для візуального відображення списку транзакцій.
    const Database& db;  ///< Посилання на базу даних для отримання транзакцій.
    QComboBox *currencyCombo; ///< Випадаючий список для вибору валюти відображення.
    QPushButton *btnUpdateRates; ///< Кнопка для оновлення курсів валют з API.
    CurrencyConverter converter; ///< Конвертер валют для перетворення сум.
};
