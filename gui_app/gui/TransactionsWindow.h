#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include "Logic.h"

/**
 * @file TransactionsWindow.h
 * @brief Оголошення діалогового вікна TransactionsWindow для додавання транзакцій.
 */

/**
 * @class TransactionsWindow
 * @brief Вікно для додавання нових транзакцій та перегляду списку існуючих.
 *
 * Це вікно містить форму для введення даних про нову транзакцію (ID, сума,
 * категорія, опис) та таблицю для відображення всіх транзакцій.
 */
class TransactionsWindow : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класу TransactionsWindow.
     * @param user Посилання на об'єкт поточного користувача.
     * @param db Посилання на об'єкт бази даних для додавання та читання транзакцій.
     * @param parent Вказівник на батьківський віджет (зазвичай 0 або nullptr).
     */
    TransactionsWindow(User &user, Database &db, QWidget *parent = nullptr);

private slots:
    /**
     * @brief Слот, що спрацьовує при натисканні кнопки "Додати".
     *
     * Зчитує дані з полів введення (idInput, amountInput тощо),
     * створює нову транзакцію та додає її до бази даних.
     */
    void onAddTransaction();

    /**
     * @brief Слот для оновлення вмісту таблиці.
     *
     * Зазвичай викликається після додавання нової транзакції.
     */
    void onRefreshTable();

    /**
     * @brief Слот для закриття поточного діалогового вікна.
     */
    void onClose();

private:
    /**
     * @brief Допоміжна функція для заповнення таблиці даними з бази даних.
     *
     * Очищує таблицю та завантажує в неї актуальний список транзакцій.
     */
    void fillTable();

    User &user;     ///< Посилання на активного користувача.
    Database &db; ///< Посилання на базу даних.

    QTableWidget *table;      ///< Таблиця для відображення транзакцій.
    QLineEdit *idInput;       ///< Поле введення для ID транзакції.
    QLineEdit *amountInput;   ///< Поле введення для суми.
    QLineEdit *categoryInput; ///< Поле введення для категорії.
    QLineEdit *descInput;     ///< Поле введення для опису.
};
