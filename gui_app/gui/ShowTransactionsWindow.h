#pragma once
#include <QDialog>
#include <QTableWidget>
#include "Database.h"

class ShowTransactionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ShowTransactionsWindow(const Database& db, QWidget *parent = nullptr);

private:
    QTableWidget *table;
    const Database& db;
};
