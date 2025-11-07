#pragma once

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "Logic.h"

/**
 * @file DataWindow.h
 * @brief Оголошення діалогового вікна DataWindow для імпорту та експорту даних.
 */

/**
 * @class DataWindow
 * @brief Діалогове вікно, що надає користувачу функції імпорту та експорту даних у форматі CSV.
 *
 * Це вікно взаємодіє з @c User та @c Database для обробки файлових операцій.
 */
class DataWindow : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класу DataWindow.
     * @param user Посилання на об'єкт поточного користувача, чиї дані будуть оброблятися.
     * @param db Посилання на об'єкт бази даних для виконання операцій.
     * @param parent Вказівник на батьківський віджет (зазвичай 0 або nullptr).
     */
    DataWindow(User &user, Database &db, QWidget *parent = nullptr);

private slots:
    /**
     * @brief Слот, що спрацьовує при натисканні кнопки "Імпорт CSV".
     *
     * Відкриває системний діалог для вибору CSV-файлу та запускає процес імпорту.
     */
    void onImportCSV();

    /**
     * @brief Слот, що спрацьовує при натисканні кнопки "Експорт CSV".
     *
     * Відкриває системний діалог для збереження даних у CSV-файл.
     */
    void onExportCSV();

    /**
     * @brief Слот для закриття поточного діалогового вікна.
     *
     * Зазвичай прив'язаний до кнопки "Закрити" або "Скасувати".
     */
    void onClose();

private:
    User &user; ///< Посилання на активного користувача для операцій з даними.
    Database &db; ///< Посилання на базу даних програми.
    QLabel *statusLabel; ///< Мітка для відображення повідомлень про стан (напр., "Успішно").
};
