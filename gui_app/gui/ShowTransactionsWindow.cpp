/**
 * @file ShowTransactionsWindow.cpp
 * @brief Реалізація діалогового вікна ShowTransactionsWindow.
 */

#include "ShowTransactionsWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidget>
#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include "Database.h"
#include "Transaction.h"
#include "CurrencyConverter.h"

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
    resize(700, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *currencyLayout = new QHBoxLayout();
    QLabel *currencyLabel = new QLabel(tr("Display Currency:"));
    currencyCombo = new QComboBox();
    currencyCombo->addItems({"USD", "EUR", "GBP", "JPY", "CAD", "AUD", "CHF", "CNY", "UAH"});
    currencyCombo->setCurrentText("USD");
    
    btnUpdateRates = new QPushButton(tr("Update Exchange Rates"));
    btnUpdateRates->setToolTip(tr("Fetch latest exchange rates from API"));
    
    currencyLayout->addWidget(currencyLabel);
    currencyLayout->addWidget(currencyCombo);
    currencyLayout->addWidget(btnUpdateRates);
    currencyLayout->addStretch();

    table = new QTableWidget(this);
    table->setColumnCount(4);

    QStringList headers;
    headers << tr("Date") << tr("Category") << tr("Amount") << tr("Currency");
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);

    converter.baseCurrency = "USD";
    converter.updateRatesIfNeeded();

    connect(btnUpdateRates, &QPushButton::clicked, this, &ShowTransactionsWindow::onUpdateExchangeRates);
    connect(currencyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ShowTransactionsWindow::onCurrencyChanged);

    layout->addLayout(currencyLayout);
    layout->addWidget(table);

    // Fill table with data
    refreshTable();
}

void ShowTransactionsWindow::refreshTable() {
    auto transactions = db.queryTransactions();
    table->setRowCount(transactions.size());

    QString displayCurrency = currencyCombo->currentText();

    for (int i = 0; i < transactions.size(); ++i) {
        const auto& t = transactions[i];

        QString dateStr = QDateTime::fromSecsSinceEpoch(t.date)
                              .toString("yyyy-MM-dd HH:mm");

        table->setItem(i, 0, new QTableWidgetItem(dateStr));
        table->setItem(i, 1, new QTableWidgetItem(
                                 QString::fromStdString(t.categoryId)
                                 ));

        std::string txCurrency = t.currency.empty() ? "USD" : t.currency;
        double convertedAmount = t.amount;
        
        if (displayCurrency.toStdString() != txCurrency) {

            if (converter.exchangeRate.find(displayCurrency.toStdString()) == converter.exchangeRate.end()) {
                converter.updateRatesIfNeeded();
            }
            convertedAmount = converter.convert(t.amount, txCurrency, displayCurrency.toStdString());
        }

        QString amountStr = QString("%1 %2").arg(convertedAmount, 0, 'f', 2).arg(displayCurrency);
        table->setItem(i, 2, new QTableWidgetItem(amountStr));

        QString originalCurrency = QString::fromStdString(txCurrency);
        table->setItem(i, 3, new QTableWidgetItem(originalCurrency));
    }
}

void ShowTransactionsWindow::onUpdateExchangeRates() {
    btnUpdateRates->setEnabled(false);
    btnUpdateRates->setText(tr("Updating..."));
    
    bool success = converter.fetchRatesFromAPI();
    
    btnUpdateRates->setEnabled(true);
    btnUpdateRates->setText(tr("Update Exchange Rates"));
    
    if (success) {
        QMessageBox::information(this, tr("Success"), 
                                 tr("Exchange rates updated successfully!"));
        refreshTable();
    } else {
        QMessageBox::warning(this, tr("Error"), 
                            tr("Failed to update exchange rates. Please check your internet connection or try again later."));
    }
}

void ShowTransactionsWindow::onCurrencyChanged() {
    refreshTable();
}
