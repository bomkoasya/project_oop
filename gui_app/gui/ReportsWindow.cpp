/**
 * @file ReportsWindow.cpp
 * @brief Реалізація діалогового вікна ReportsWindow для генерації звітів.
 */

#include "ReportsWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QComboBox>
#include <QStringBuilder> // Для оператора %
#include <iomanip>
#include <sstream>
#include "Logic.h"
#include "ReportGenerator.h"
#include "CurrencyConverter.h"

/**
 * @brief Конструктор ReportsWindow.
 *
 * Ініціалізує елементи інтерфейсу: заголовок, текстове поле 'output'
 * та кнопки для генерації звіту, експорту та закриття.
 * Встановлює з'єднання сигналів та слотів.
 */
ReportsWindow::ReportsWindow(const User &u, QWidget *parent)
    : QDialog(parent), user(u), selectedCurrency("USD") {

    setWindowTitle(tr("Reports Generator"));
    resize(600, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *title = new QLabel(tr("Report Generator"));
    title->setAlignment(Qt::AlignCenter);

    QHBoxLayout *currencyLayout = new QHBoxLayout();
    QLabel *currencyLabel = new QLabel(tr("Display Currency:"));
    currencyCombo = new QComboBox();
    currencyCombo->addItems({"USD", "EUR", "GBP", "JPY", "CAD", "AUD", "CHF", "CNY", "UAH"});
    currencyCombo->setCurrentText(selectedCurrency);
    
    btnUpdateRates = new QPushButton(tr("Update Exchange Rates"));
    btnUpdateRates->setToolTip(tr("Fetch latest exchange rates from API"));
    
    currencyLayout->addWidget(currencyLabel);
    currencyLayout->addWidget(currencyCombo);
    currencyLayout->addWidget(btnUpdateRates);
    currencyLayout->addStretch();

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setPlaceholderText(tr("Generated report will appear here..."));

    QPushButton *btnFull = new QPushButton(tr("Generate Full Report"));
    QPushButton *btnCSV = new QPushButton(tr("Export to CSV"));
    QPushButton *btnJSON = new QPushButton(tr("Export to JSON"));
    QPushButton *btnClose = new QPushButton(tr("Close"));

    layout->addWidget(title);
    layout->addLayout(currencyLayout);
    layout->addWidget(output);
    layout->addWidget(btnFull);
    layout->addWidget(btnCSV);
    layout->addWidget(btnJSON);
    layout->addWidget(btnClose);

    converter.baseCurrency = "USD";

    converter.updateRatesIfNeeded();

    connect(btnFull, &QPushButton::clicked, this, &ReportsWindow::onGenerateFullReport);
    connect(btnCSV, &QPushButton::clicked, this, &ReportsWindow::onExportCSV);
    connect(btnJSON, &QPushButton::clicked, this, &ReportsWindow::onExportJSON);
    connect(btnClose, &QPushButton::clicked, this, &ReportsWindow::onClose);
    connect(btnUpdateRates, &QPushButton::clicked, this, &ReportsWindow::onUpdateExchangeRates);
    connect(currencyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ReportsWindow::onCurrencyChanged);
}

/**
 * @brief Генерує повний текстовий звіт та відображає його у полі 'output'.
 *
 * 1. Перевіряє, чи є транзакції у користувача.
 * 2. Створює екземпляр ReportGenerator.
 * 3. Розраховує загальну суму.
 * 4. Форматує рядок звіту (QString) з заголовком, статистикою та списком транзакцій.
 * 5. Встановлює згенерований текст у QTextEdit 'output'.
 */
void ReportsWindow::onGenerateFullReport() {
    if (user.transactions.empty()) {
        QMessageBox::information(this, tr("No Data"), tr("No transactions to generate a report."));
        return;
    }

    refreshReport();
}

void ReportsWindow::refreshReport() {
    if (user.transactions.empty()) {
        return;
    }

    ReportGenerator rg(tr("User Finance Report").toStdString());
    rg.transactions = user.transactions;

    double total = rg.calculateTotal();

    QString displayCurrency = currencyCombo->currentText();
    double convertedTotal = total;
    QString currencySymbol = displayCurrency;

    if (!displayCurrency.isEmpty() && displayCurrency != "USD") {

        if (converter.exchangeRate.find(displayCurrency.toStdString()) == converter.exchangeRate.end()) {

            converter.updateRatesIfNeeded();
        }

        std::string transactionCurrency = "USD";
        if (!user.transactions.empty() && !user.transactions[0].currency.empty()) {
            transactionCurrency = user.transactions[0].currency;
        }
        
        convertedTotal = converter.convert(total, transactionCurrency, displayCurrency.toStdString());
    }

    QString report;

    report += "<h2 align='center'>" + tr("FINANCE REPORT") + "</h2>";
    report += "<hr>"; // Горизонтальна лінія
    report += "<p><b>" + tr("User:") + "</b> " + QString::fromStdString(user.name) + "</p>";
    report += "<p><b>" + tr("Total Transactions:") + "</b> " + QString::number(user.transactions.size()) + "</p>";

    report += "<p><b>" + tr("Total Amount:") + "</b> <span style='color: green; font-size: 14pt;'>"
              + QString::number(convertedTotal, 'f', 2) + " " + currencySymbol + "</span>";

    if (displayCurrency != "USD") {
        report += " <small>(Original: " + QString::number(total, 'f', 2) + " USD)</small>";
    }
    report += "</p><br>";

    // Таблиця транзакцій
    report += "<h3 align='center'>" + tr("Transactions List") + "</h3>";
    report += "<table border='1' cellspacing='0' cellpadding='5' width='100%' style='border-collapse: collapse;'>";

    // Заголовки таблиці
    report += "<tr style='background-color: #f2f2f2;'>"
              "<th>ID</th>"
              "<th>" + tr("Amount") + "</th>"
                               "<th>" + tr("Category") + "</th>"
                                 "<th>" + tr("Description") + "</th>"
                                    "</tr>";

    for (const auto &t : user.transactions) {
        std::string txCurrency = t.currency.empty() ? "USD" : t.currency;
        double convertedAmount = t.amount;

        if (displayCurrency.toStdString() != txCurrency) {
            convertedAmount = converter.convert(t.amount, txCurrency, displayCurrency.toStdString());
        }

        QString colorStyle = (convertedAmount < 0) ? "color: red;" : "color: navy;";

        report += "<tr>";
        report += "<td align='center'>" + QString::fromStdString(t.id) + "</td>";
        report += "<td align='right' style='" + colorStyle + "'><b>"
                  + QString::number(convertedAmount, 'f', 2) + " " + currencySymbol + "</b></td>";
        report += "<td align='center'>" + QString::fromStdString(t.categoryId) + "</td>";
        report += "<td>" + QString::fromStdString(t.description) + "</td>";
        report += "</tr>";
    }
    report += "</table>";

    output->setHtml(report);
    selectedCurrency = displayCurrency;
}

void ReportsWindow::onUpdateExchangeRates() {
    btnUpdateRates->setEnabled(false);
    btnUpdateRates->setText(tr("Updating..."));

    bool success = converter.fetchRatesFromAPI();
    
    btnUpdateRates->setEnabled(true);
    btnUpdateRates->setText(tr("Update Exchange Rates"));
    
    if (success) {
        QMessageBox::information(this, tr("Success"), 
                                 tr("Exchange rates updated successfully!"));
        if (!output->toPlainText().isEmpty()) {
            refreshReport();
        }
    } else {
        QMessageBox::warning(this, tr("Error"), 
                            tr("Failed to update exchange rates. Please check your internet connection or try again later."));
    }
}

void ReportsWindow::onCurrencyChanged() {

    if (!output->toPlainText().isEmpty()) {
        refreshReport();
    }
}

/**
 * @brief Експортує дані користувача у файл CSV.
 *
 * 1. Перевіряє, чи є транзакції.
 * 2. Відкриває QFileDialog, щоб користувач обрав шлях збереження.
 * 3. Створює ReportGenerator та викликає його метод exportToCSV().
 * 4. Показує повідомлення про успіх.
 */
void ReportsWindow::onExportCSV() {
    if (user.transactions.empty()) {
        QMessageBox::warning(this, tr("No Data"), tr("There are no transactions to export."));
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Export to CSV"),
                                                    "",
                                                    tr("CSV files (*.csv)"));
    if (filePath.isEmpty()) return;

    ReportGenerator rg(tr("User Finance Report").toStdString());
    rg.transactions = user.transactions;
    rg.exportToCSV(filePath.toStdString());

    QMessageBox::information(this, tr("Export Successful"), tr("Report exported to CSV successfully!"));
}

/**
 * @brief Експортує дані користувача у файл JSON.
 *
 * 1. Перевіряє, чи є транзакції.
 * 2. Відкриває QFileDialog, щоб користувач обрав шлях збереження.
 * 3. Створює ReportGenerator та викликає його метод exportToJSON().
 * 4. Показує повідомлення про успіх.
 */
void ReportsWindow::onExportJSON() {
    if (user.transactions.empty()) {
        QMessageBox::warning(this, tr("No Data"), tr("There are no transactions to export."));
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Export to JSON"),
                                                    "",
                                                    tr("JSON files (*.json)"));
    if (filePath.isEmpty()) return;

    ReportGenerator rg(tr("User Finance Report").toStdString());
    rg.transactions = user.transactions;
    rg.exportToJSON(filePath.toStdString());

    QMessageBox::information(this, tr("Export Successful"), tr("Report exported to JSON successfully!"));
}

/**
 * @brief Закриває діалогове вікно.
 */
void ReportsWindow::onClose() {
    close();
}
