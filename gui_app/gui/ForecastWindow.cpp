/**
 * @file ForecastWindow.cpp
 * @brief Реалізація діалогового вікна ForecastWindow для фінансових прогнозів.
 */

#include "ForecastWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <iomanip>
#include <sstream>
#include <vector>
#include "Database.h"
#include "Transaction.h"
#include "ForecastStrategy.h" // Містить MovingAverageStrategy

/**
 * @brief Конструктор ForecastWindow.
 *
 * Ініціалізує елементи інтерфейсу: заголовок, текстове поле для
 * виводу (output) та кнопки. Встановлює з'єднання сигналів та слотів.
 */
ForecastWindow::ForecastWindow(const Database& db, QWidget *parent)
    : QDialog(parent), db(db)
{
    setWindowTitle(tr("Forecast"));
    resize(500, 350);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel(tr("Forecast Based on Transaction History"));
    title->setAlignment(Qt::AlignCenter);

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setPlaceholderText(tr("Forecast result will appear here..."));

    QPushButton *btnForecast = new QPushButton(tr("Generate Forecast"));
    QPushButton *btnClose = new QPushButton(tr("Close"));

    layout->addWidget(title);
    layout->addWidget(output);
    layout->addWidget(btnForecast);
    layout->addWidget(btnClose);

    connect(btnForecast, &QPushButton::clicked, this, &ForecastWindow::onForecastClicked);
    connect(btnClose, &QPushButton::clicked, this, &ForecastWindow::onCloseClicked);
}

/**
 * @brief Обробляє логіку генерації прогнозу при натисканні кнопки.
 *
 * 1. Перевіряє, чи є дані в базі.
 * 2. Відкриває QInputDialog, щоб отримати від користувача кількість транзакцій для аналізу (період ковзного середнього).
 * 3. Створює об'єкт стратегії (MovingAverageStrategy).
 * 4. Отримує історію транзакцій з БД.
 * 5. Викликає метод forecast() для отримання прогнозованого значення.
 * 6. Форматує та виводить результат у текстове поле 'output'.
 */
void ForecastWindow::onForecastClicked() {
    // 1. Перевірка наявності даних
    if (db.txStore.empty()) {
        QMessageBox::warning(this, tr("No Data"), tr("Not enough data to make a forecast."));
        return;
    }

    // 2. Отримання вводу від користувача
    bool ok;
    int months = QInputDialog::getInt(this, tr("Forecast Input"),
                                      tr("Enter number of recent transactions to analyze:"),
                                      5, 1, db.txStore.size(), 1, &ok);
    if (!ok) return; // Користувач натиснув "Cancel"

    // 3-5. Розрахунок прогнозу
    MovingAverageStrategy strategy;
    std::vector<Transaction> history = db.queryTransactions();
    double prediction = strategy.forecast(history, months);

    // 6. Форматування та вивід
    QString resultText = tr("Based on the last %1 transactions,\n"
                            "the predicted next transaction amount is: %2")
                             .arg(months)
                             .arg(prediction, 0, 'f', 2);

    output->setPlainText(resultText);
}

/**
 * @brief Закриває діалогове вікно.
 */
void ForecastWindow::onCloseClicked() {
    close();
}
