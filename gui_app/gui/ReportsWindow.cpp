#include "ReportsWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <iomanip>
#include <sstream>
#include "Logic.h"
#include "ReportGenerator.h"


ReportsWindow::ReportsWindow(const User &u, QWidget *parent)
    : QDialog(parent), user(u) {
    setWindowTitle("Reports Generator");
    resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *title = new QLabel("Report Generator");
    title->setAlignment(Qt::AlignCenter);

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setPlaceholderText("Generated report will appear here...");

    QPushButton *btnFull = new QPushButton("Generate Full Report");
    QPushButton *btnCSV = new QPushButton("Export to CSV");
    QPushButton *btnJSON = new QPushButton("Export to JSON");
    QPushButton *btnClose = new QPushButton("Close");

    layout->addWidget(title);
    layout->addWidget(output);
    layout->addWidget(btnFull);
    layout->addWidget(btnCSV);
    layout->addWidget(btnJSON);
    layout->addWidget(btnClose);

    connect(btnFull, &QPushButton::clicked, this, &ReportsWindow::onGenerateFullReport);
    connect(btnCSV, &QPushButton::clicked, this, &ReportsWindow::onExportCSV);
    connect(btnJSON, &QPushButton::clicked, this, &ReportsWindow::onExportJSON);
    connect(btnClose, &QPushButton::clicked, this, &ReportsWindow::onClose);
}

void ReportsWindow::onGenerateFullReport() {
    if (user.transactions.empty()) {
        QMessageBox::information(this, "No Data", "No transactions to generate a report.");
        return;
    }

    ReportGenerator rg("User Finance Report");
    rg.transactions = user.transactions;

    double total = rg.calculateTotal();

    std::ostringstream report;
    report << "=== FINANCE REPORT ===\n";
    report << "User: " << user.name << "\n";
    report << "Total Transactions: " << user.transactions.size() << "\n";
    report << "Total Amount: " << std::fixed << std::setprecision(2) << total << "\n\n";
    report << "Transactions:\n";

    for (const auto &t : user.transactions) {
        report << "- ID: " << t.id
               << ", Amount: " << t.amount
               << ", Category: " << t.categoryId
               << ", Description: " << t.description << "\n";
    }

    output->setPlainText(QString::fromStdString(report.str()));
}

void ReportsWindow::onExportCSV() {
    if (user.transactions.empty()) {
        QMessageBox::warning(this, "No Data", "There are no transactions to export.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Export to CSV", "", "CSV files (*.csv)");
    if (filePath.isEmpty()) return;

    ReportGenerator rg("User Finance Report");
    rg.transactions = user.transactions;
    rg.exportToCSV(filePath.toStdString());

    QMessageBox::information(this, "Export Successful", "Report exported to CSV successfully!");
}

void ReportsWindow::onExportJSON() {
    if (user.transactions.empty()) {
        QMessageBox::warning(this, "No Data", "There are no transactions to export.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Export to JSON", "", "JSON files (*.json)");
    if (filePath.isEmpty()) return;

    ReportGenerator rg("User Finance Report");
    rg.transactions = user.transactions;
    rg.exportToJSON(filePath.toStdString());

    QMessageBox::information(this, "Export Successful", "Report exported to JSON successfully!");
}

void ReportsWindow::onClose() {
    close();
}
