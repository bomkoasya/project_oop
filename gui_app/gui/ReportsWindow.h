#pragma once

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include "Logic.h"
#include "CurrencyConverter.h"

/**
 * @file ReportsWindow.h
 * @brief Оголошення діалогового вікна ReportsWindow для генерації звітів.
 */

/**
 * @class ReportsWindow
 * @brief Вікно для генерації та відображення звітів, а також їх експорту у CSV/JSON.
 *
 * Це вікно надає користувачу інтерфейс для перегляду повного звіту
 * про свої фінанси та збереження цього звіту у різних форматах.
 */
class ReportsWindow : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класу ReportsWindow.
     * @param user Посилання на об'єкт користувача (лише для читання), дані якого використовуються для звітів.
     * @param parent Вказівник на батьківський віджет (зазвичай 0 або nullptr).
     */
    ReportsWindow(const User &user, QWidget *parent = nullptr);

private slots:
    /**
     * @brief Слот, що спрацьовує при натисканні "Сформувати повний звіт".
     *
     * Генерує звіт на основі даних користувача та виводить його у текстове поле 'output'.
     */
    void onGenerateFullReport();

    /**
     * @brief Слот для експорту поточного звіту у формат CSV.
     *
     * Використовує шлях, вказаний у 'pathInput', або запитує новий.
     */
    void onExportCSV();

    /**
     * @brief Слот для експорту поточного звіту у формат JSON.
     *
     * Використовує шлях, вказаний у 'pathInput', або запитує новий.
     */
    void onExportJSON();

    /**
     * @brief Слот для закриття поточного діалогового вікна.
     */
    void onClose();

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
     * @brief Оновлює відображення звіту з урахуванням вибраної валюти.
     */
    void refreshReport();

    const User &user; ///< Посилання (лише для читання) на користувача, для якого генерується звіт.
    QTextEdit *output;  ///< Текстове поле для відображення згенерованого звіту.
    QLineEdit *pathInput; ///< Поле для введення шляху збереження файлу (для експорту).
    QComboBox *currencyCombo; ///< Випадаючий список для вибору валюти відображення.
    QPushButton *btnUpdateRates; ///< Кнопка для оновлення курсів валют з API.
    CurrencyConverter converter; ///< Конвертер валют для перетворення сум.
    QString selectedCurrency; ///< Поточна вибрана валюта для відображення.
};
