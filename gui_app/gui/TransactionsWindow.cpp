#include "TransactionsWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>
#include "Logic.h"
#include "Database.h"
#include "Transaction.h"


TransactionsWindow::TransactionsWindow(User &u, Database &d, QWidget *parent)
    : QDialog(parent), user(u), db(d) {

    setWindowTitle(tr("Transactions Manager"));
    resize(700, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(4);

    QStringList headers;
    headers << tr("ID") << tr("Amount") << tr("Category") << tr("Description");
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QFormLayout *formLayout = new QFormLayout();
    idInput = new QLineEdit();
    amountInput = new QLineEdit();
    categoryInput = new QLineEdit();
    descInput = new QLineEdit();

    formLayout->addRow(tr("ID:"), idInput);
    formLayout->addRow(tr("Amount:"), amountInput);
    formLayout->addRow(tr("Category:"), categoryInput);
    formLayout->addRow(tr("Description:"), descInput);

    QPushButton *btnAdd = new QPushButton(tr("Add Transaction"));
    QPushButton *btnRefresh = new QPushButton(tr("Refresh"));
    QPushButton *btnClose = new QPushButton(tr("Close"));

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnRefresh);
    btnLayout->addWidget(btnClose);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(table);

    connect(btnAdd, &QPushButton::clicked, this, &TransactionsWindow::onAddTransaction);
    connect(btnRefresh, &QPushButton::clicked, this, &TransactionsWindow::onRefreshTable);
    connect(btnClose, &QPushButton::clicked, this, &TransactionsWindow::onClose);

    fillTable();
}

void TransactionsWindow::fillTable() {
    table->setRowCount(static_cast<int>(user.transactions.size()));

    int row = 0;
    for (const auto &t : user.transactions) {
        table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(t.id)));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(t.amount, 'f', 2)));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(t.categoryId)));
        table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(t.description)));
        ++row;
    }
}

void TransactionsWindow::onAddTransaction() {
    if (idInput->text().isEmpty() || amountInput->text().isEmpty()) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please enter ID and Amount."));
        return;
    }

    Transaction t;
    t.id = idInput->text().toStdString();
    t.amount = amountInput->text().toDouble();
    t.categoryId = categoryInput->text().toStdString();
    t.description = descInput->text().toStdString();
    t.currency = user.defaultCurrency;
    t.date = time(nullptr);

    user.transactions.push_back(t);
    db.persistTransaction(t);

    fillTable();

    idInput->clear();
    amountInput->clear();
    categoryInput->clear();
    descInput->clear();

    QMessageBox::information(this, tr("Success"), tr("Transaction added!"));
}

void TransactionsWindow::onRefreshTable() {
    fillTable();
}

void TransactionsWindow::onClose() {
    close();
}
