#include "ShowTransactionsWindow.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QString>
#include <QDateTime>


ShowTransactionsWindow::ShowTransactionsWindow(const Database& db, QWidget *parent)
    : QDialog(parent), db(db)
{
    setWindowTitle("Transactions");
    resize(600, 400);

    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Date", "Category", "Amount"});
    table->horizontalHeader()->setStretchLastSection(true);

    auto transactions = db.queryTransactions();
    table->setRowCount(transactions.size());

    for (int i = 0; i < transactions.size(); ++i) {

        QString dateStr = QDateTime::fromSecsSinceEpoch(transactions[i].date)
                              .toString("yyyy-MM-dd HH:mm");

        table->setItem(i, 0, new QTableWidgetItem(dateStr));
        table->setItem(i, 1, new QTableWidgetItem(
                                 QString::fromStdString(transactions[i].categoryId)
                                 ));
        table->setItem(i, 2, new QTableWidgetItem(
                                 QString::number(transactions[i].amount, 'f', 2)
                                 ));
    }


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(table);
}
