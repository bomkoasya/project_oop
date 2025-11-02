#include "DataWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <vector>
#include "Importer.h"
#include "Exporter.h"
#include "Transaction.h"
#include "Result.h"
#include "Database.h"
using namespace std;


DataWindow::DataWindow(User &u, Database &d, QWidget *parent)
    : QDialog(parent), user(u), db(d) {
    setWindowTitle("Data Import / Export");
    resize(500, 250);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Import or Export Transactions");
    title->setAlignment(Qt::AlignCenter);

    QPushButton *btnImport = new QPushButton("Import from CSV");
    QPushButton *btnExport = new QPushButton("Export to CSV");
    QPushButton *btnClose = new QPushButton("Close");

    statusLabel = new QLabel("Ready");
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

void DataWindow::onImportCSV() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select CSV File", "", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    CSVImporter importer(filePath.toStdString());
    Result<vector<Transaction>> result = importer.import();

    if (result.is_success()) {
        vector<Transaction> imported = result.get_value();
        user.transactions.insert(user.transactions.end(), imported.begin(), imported.end());
        for (const auto &t : imported)
            db.persistTransaction(t);

        statusLabel->setText(QString::number(imported.size()) + " transactions imported successfully!");
        QMessageBox::information(this, "Import Successful", "Transactions imported successfully!");
    } else {
        statusLabel->setText("Import failed: " + QString::fromStdString(result.get_error()));
        QMessageBox::critical(this, "Error", "Failed to import transactions.");
    }
}

void DataWindow::onExportCSV() {
    if (user.transactions.empty()) {
        QMessageBox::warning(this, "No Data", "No transactions to export.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save CSV File", "", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    CSVExporter exporter(filePath.toStdString());
    if (exporter.exportData(user.transactions)) {
        statusLabel->setText("Transactions exported successfully!");
        QMessageBox::information(this, "Export Successful", "Transactions exported to CSV.");
    } else {
        statusLabel->setText("Export failed.");
        QMessageBox::critical(this, "Error", "Failed to export transactions.");
    }
}

void DataWindow::onClose() {
    close();
}
