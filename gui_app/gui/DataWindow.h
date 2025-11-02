#pragma once
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "Logic.h"

class DataWindow : public QDialog {
    Q_OBJECT

public:
    DataWindow(User &user, Database &db, QWidget *parent = nullptr);

private slots:
    void onImportCSV();
    void onExportCSV();
    void onClose();

private:
    User &user;
    Database &db;
    QLabel *statusLabel;
};
