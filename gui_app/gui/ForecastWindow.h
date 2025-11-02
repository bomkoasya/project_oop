#pragma once

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

#include "Database.h"
#include "MovingAverageStrategy.h"

class ForecastWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ForecastWindow(const Database& db, QWidget *parent = nullptr);

private slots:
    void onForecastClicked();
    void onCloseClicked();

private:
    QTextEdit* output;
    const Database& db;
};
