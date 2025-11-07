#include "DataWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <vector>
#include "Importer.h"  // Ваші заголовки
#include "Exporter.h"
#include "Transaction.h"
#include "Result.h"
#include "Database.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

using namespace std;


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
            db.persistTransaction(t);

        statusLabel->setText(tr("%1 transactions imported successfully!").arg(imported.size()));
        QMessageBox::information(this, tr("Import Successful"), tr("Transactions imported successfully!"));
    } else {
        statusLabel->setText(tr("Import failed: ") + QString::fromStdString(result.get_error()));
        QMessageBox::critical(this, tr("Error"), tr("Failed to import transactions."));
    }
}

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

void DataWindow::onClose() {
    close();
}
