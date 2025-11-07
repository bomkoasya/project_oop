#include "ReportsWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringBuilder>
#include <iomanip>
#include <sstream>
#include "Logic.h"
#include "ReportGenerator.h"


ReportsWindow::ReportsWindow(const User &u, QWidget *parent)
    : QDialog(parent), user(u) {

    setWindowTitle(tr("Reports Generator"));
    resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *title = new QLabel(tr("Report Generator"));
    title->setAlignment(Qt::AlignCenter);

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setPlaceholderText(tr("Generated report will appear here..."));

    QPushButton *btnFull = new QPushButton(tr("Generate Full Report"));
    QPushButton *btnCSV = new QPushButton(tr("Export to CSV"));
    QPushButton *btnJSON = new QPushButton(tr("Export to JSON"));
    QPushButton *btnClose = new QPushButton(tr("Close"));

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
        QMessageBox::information(this, tr("No Data"), tr("No transactions to generate a report."));
        return;
    }

    ReportGenerator rg(tr("User Finance Report").toStdString());
    rg.transactions = user.transactions;

    double total = rg.calculateTotal();

    QString report;

    report = tr("=== FINANCE REPORT ===\n") %
             tr("User: %1\n").arg(QString::fromStdString(user.name)) %
             tr("Total Transactions: %1\n").arg(user.transactions.size()) %
             tr("Total Amount: %1\n\n").arg(total, 0, 'f', 2) %
             tr("Transactions:\n");

    for (const auto &t : user.transactions) {
        report += tr("- ID: %1, Amount: %2, Category: %3, Description: %4\n")
                      .arg(t.id)
                      .arg(t.amount, 0, 'f', 2)
                      .arg(QString::fromStdString(t.categoryId))
                      .arg(QString::fromStdString(t.description));
    }

    output->setPlainText(report);
}

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

void ReportsWindow::onClose() {
    close();
}
