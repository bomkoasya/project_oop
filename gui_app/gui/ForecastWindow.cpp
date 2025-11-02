#include "ForecastWindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <iomanip>
#include <sstream>

ForecastWindow::ForecastWindow(const Database& db, QWidget *parent)
    : QDialog(parent), db(db)
{
    setWindowTitle("Forecast");
    resize(500, 350);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Forecast Based on Transaction History");
    title->setAlignment(Qt::AlignCenter);

    output = new QTextEdit();
    output->setReadOnly(true);
    output->setPlaceholderText("Forecast result will appear here...");

    QPushButton *btnForecast = new QPushButton("Generate Forecast");
    QPushButton *btnClose = new QPushButton("Close");

    layout->addWidget(title);
    layout->addWidget(output);
    layout->addWidget(btnForecast);
    layout->addWidget(btnClose);

    connect(btnForecast, &QPushButton::clicked, this, &ForecastWindow::onForecastClicked);
    connect(btnClose, &QPushButton::clicked, this, &ForecastWindow::onCloseClicked);
}

void ForecastWindow::onForecastClicked() {
    if (db.txStore.empty()) {
        QMessageBox::warning(this, "No Data", "Not enough data to make a forecast.");
        return;
    }

    bool ok;
    int months = QInputDialog::getInt(this, "Forecast Input",
                                      "Enter number of recent transactions to analyze:",
                                      5, 1, db.txStore.size(), 1, &ok);
    if (!ok) return;

    MovingAverageStrategy strategy;
    std::vector<Transaction> history = db.queryTransactions();
    double prediction = strategy.forecast(history, months);

    std::ostringstream ss;
    ss << "Based on the last " << months << " transactions,\n"
       << "the predicted next transaction amount is: "
       << std::fixed << std::setprecision(2) << prediction;

    output->setPlainText(QString::fromStdString(ss.str()));
}

void ForecastWindow::onCloseClicked() {
    close();
}
