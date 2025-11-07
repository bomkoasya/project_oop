/**
 * @file DataWindow.cpp
 * @brief Реалізація діалогового вікна DataWindow для імпорту та експорту даних.
 */

#include "DataWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <vector>
#include "Importer.h"
#include "Exporter.h"
#include "Transaction.h"
#include "Result.h"
#include "Database.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

using namespace std;

/**
 * @brief Конструктор DataWindow.
 *
 * Ініціалізує елементи інтерфейсу (кнопки, мітки) та
 * налаштовує їх компонування (layout) і з'єднання сигналів та слотів.
 */
DataWindow::DataWindow(User &u, Database &d, QWidget *parent)
    : QDialog(parent), user(u), db(d) {

    setWindowTitle(tr("Data Import / Export"));
    resize(500, 250);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel(tr("Import or Export Transactions"));
    title->setAlignment(Qt::AlignCenter);

    QPushButton *btnImport = new QPushButton(tr("Import from CSV"));
    QPushButton *btnExport = new QPushButton(tr("Export to CSV"));
    QPushButton *btnClose = new QPushButton(tr("Close"));

    statusLabel = new QLabel(tr("Ready"));
    statusLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(title);
    layout->addWidget(btnImport);
    layout->addWidget(btnExport);
    layout->addSpacing(20);
    layout->addWidget(statusLabel);
    layout->addSpacing(10);
    layout->addWidget(btnClose);

    connect(btnImport, &QPushButton::clicked, this, &DataWindow::onImportCSV);
    connect(btnExport, &QPushButton::clicked, this, &DataWindow::onExportCSV);
    connect(btnClose, &QPushButton::clicked, this, &DataWindow::onClose);
}

/**
 * @brief Обробляє логіку імпорту з CSV файлу.
 *
 * 1. Відкриває QFileDialog, щоб користувач обрав файл.
 * 2. Використовує CSVImporter для парсингу файлу.
 * 3. У разі успіху:
 * - Додає транзакції до об'єкта User.
 * - Зберігає транзакції в базу даних.
 * - Показує повідомлення про успіх.
 * 4. У разі невдачі:
 * - Показує повідомлення про помилку.
 */
void DataWindow::onImportCSV() {
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Select CSV File"),
                                                    "",
                                                    tr("CSV Files (*.csv)"));
    if (filePath.isEmpty()) return;

    CSVImporter importer(filePath.toStdString());
    Result<vector<Transaction>> result = importer.import();

    if (result.is_success()) {
        vector<Transaction> imported = result.get_value();
        user.transactions.insert(user.transactions.end(), imported.begin(), imported.end());
        for (const auto &t : imported)
            db.persistTransaction(t); // Збереження в БД

        statusLabel->setText(tr("%1 transactions imported successfully!").arg(imported.size()));
        QMessageBox::information(this, tr("Import Successful"), tr("Transactions imported successfully!"));
    } else {
        statusLabel->setText(tr("Import failed: ") + QString::fromStdString(result.get_error()));
        QMessageBox::critical(this, tr("Error"), tr("Failed to import transactions."));
    }
}

/**
 * @brief Обробляє логіку експорту у CSV файл.
 *
 * 1. Перевіряє, чи є транзакції для експорту.
 * 2. Відкриває QFileDialog, щоб користувач вказав шлях для збереження.
 * 3. Використовує CSVExporter для запису даних у файл.
 * 4. Показує повідомлення про успіх або помилку.
 */
void DataWindow::onExportCSV() {
    if (user.transactions.empty()) {
        QMessageBox::warning(this, tr("No Data"), tr("No transactions to export."));
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("Save CSV File"),
                                                    "",
                                                    tr("CSV Files (*.csv)"));
    if (filePath.isEmpty()) return;

    CSVExporter exporter(filePath.toStdString());
    if (exporter.exportData(user.transactions)) {
        statusLabel->setText(tr("Transactions exported successfully!"));
        QMessageBox::information(this, tr("Export Successful"), tr("Transactions exported to CSV."));
    } else {
        statusLabel->setText(tr("Export failed."));
        QMessageBox::critical(this, tr("Error"), tr("Failed to export transactions."));
    }
}

/**
 * @brief Закриває діалогове вікно.
 */
void DataWindow::onClose() {
    close();
}
