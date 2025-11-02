#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "Logic.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    private slots:
        void openAddTransaction();
    void openShowTransactions();
    void openReports();
    void openDataIO();
    void openForecast();
    void saveAndExit();

private:
    User user;
    Database db;
};
