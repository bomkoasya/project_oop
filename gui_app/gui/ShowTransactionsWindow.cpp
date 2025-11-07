/**
 * @file ShowTransactionsWindow.cpp
 * @brief Реалізація діалогового вікна ShowTransactionsWindow.
 */

#include "ShowTransactionsWindow.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidget>
#include <QString>
#include <QDateTime>
#include <QStringList>
#include "Database.h"
#include "Transaction.h"

/**
 * @brief Конструктор ShowTransactionsWindow.
 *
 * Ініціалізує вікно, яке відображає всі транзакції у вигляді таблиці.
 * Логіка конструктора:
 * 1. Налаштовує заголовок, розмір та саму таблицю (QTableWidget).
 * 2. Встановлює заголовки колонок ("Дата", "Категорія", "Сума").
 * 3. Отримує список всіх транзакцій з об'єкта Database.
 * 4. Встановлює кількість рядків у таблиці відповідно до кількості транзакцій.
 * 5. Ітерує по списку транзакцій та заповнює рядки таблиці даними.
 * 6. Під час заповнення форматує дату (з Unix timestamp у рядок)
 * та суму (як число з 2 знаками після коми).
 * 7. Додає готову таблицю до головного компонувальника вікна.
 */
ShowTransactionsWindow::ShowTransactionsWindow(const Database& db, QWidget *parent)
    : QDialog(parent), db(db)
{
    setWindowTitle(tr("Transactions"));
    resize(600, 400);

    // 1. Створення таблиці
    table = new QTableWidget(this);
    table->setColumnCount(3);

    // 2. Встановлення заголовків
    QStringList headers;
    headers << tr("Date") << tr("Category") << tr("Amount");
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true); // Остання колонка займає весь простір

    // 3. Отримання даних
    auto transactions = db.queryTransactions();
    table->setRowCount(transactions.size());

    // 4-6. Заповнення таблиці
    for (int i = 0; i < transactions.size(); ++i) {

        // Форматування дати з time_t (long) у QString
        QString dateStr = QDateTime::fromSecsSinceEpoch(transactions[i].date)
                              .toString("yyyy-MM-dd HH:mm");

        table->setItem(i, 0, new QTableWidgetItem(dateStr));
        table->setItem(i, 1, new QTableWidgetItem(
                                 QString::fromStdString(transactions[i].categoryId)
                                 ));
        // Форматування суми 'f' - float, 2 - знаки після коми
        table->setItem(i, 2, new QTableWidgetItem(
                                 QString::number(transactions[i].amount, 'f', 2)
                                 ));
    }

    // 7. Налаштування компонування
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(table);
}
