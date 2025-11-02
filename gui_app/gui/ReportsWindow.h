#pragma once
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include "Logic.h"

class ReportsWindow : public QDialog {
    Q_OBJECT

public:
    ReportsWindow(const User &user, QWidget *parent = nullptr);

private slots:
    void onGenerateFullReport();
    void onExportCSV();
    void onExportJSON();
    void onClose();

private:
    const User &user;
    QTextEdit *output;
    QLineEdit *pathInput;
};
