#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include "Logic.h"

class TransactionsWindow : public QDialog {
    Q_OBJECT

public:
    TransactionsWindow(User &user, Database &db, QWidget *parent = nullptr);

private slots:
    void onAddTransaction();
    void onRefreshTable();
    void onClose();

private:
    User &user;
    Database &db;
    QTableWidget *table;
    QLineEdit *idInput;
    QLineEdit *amountInput;
    QLineEdit *categoryInput;
    QLineEdit *descInput;

    void fillTable();
};
