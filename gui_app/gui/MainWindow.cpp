#include "MainWindow.h"
#include "TransactionsWindow.h"
#include "ReportsWindow.h"
#include "DataWindow.h"
#include "ReportGenerator.h"
#include "ForecastWindow.h"
#include "ShowTransactionsWindow.h"


#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Personal Finance Manager");
    resize(400, 300);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    QLabel *title = new QLabel("=== Personal Finance Manager ===");
    title->setAlignment(Qt::AlignCenter);

    QPushButton *btnAdd = new QPushButton("Add Transaction");
    QPushButton *btnShow = new QPushButton("Show All Transactions");
    QPushButton *btnReports = new QPushButton("Generate Reports");
    QPushButton *btnDataIO = new QPushButton("Data Import / Export");
    QPushButton *btnForecast = new QPushButton("Forecast");
    QPushButton *btnSaveExit = new QPushButton("Save and Exit");

    layout->addWidget(title);
    layout->addSpacing(10);
    layout->addWidget(btnAdd);
    layout->addWidget(btnShow);
    layout->addWidget(btnReports);
    layout->addWidget(btnDataIO);
    layout->addWidget(btnForecast);
    layout->addSpacing(20);
    layout->addWidget(btnSaveExit);

    setCentralWidget(central);

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::openAddTransaction);
    connect(btnShow, &QPushButton::clicked, this, &MainWindow::openShowTransactions);
    connect(btnReports, &QPushButton::clicked, this, &MainWindow::openReports);
    connect(btnDataIO, &QPushButton::clicked, this, &MainWindow::openDataIO);
    connect(btnForecast, &QPushButton::clicked, this, &MainWindow::openForecast);
    connect(btnSaveExit, &QPushButton::clicked, this, &MainWindow::saveAndExit);

    bool ok;
    QString username = QInputDialog::getText(this, "User Login",
                                             "Enter your username (profile ID):",
                                             QLineEdit::Normal, "", &ok);
    if (ok && !username.isEmpty()) {
        user.id = username.toStdString();
        user.name = user.id;
        loadUserData(user, db);
    } else {
        QMessageBox::information(this, "Info", "New empty profile created.");
    }
}

void MainWindow::openAddTransaction() {
    TransactionsWindow *w = new TransactionsWindow(user, db, this);
    w->exec();
}


void MainWindow::openShowTransactions() {
    ShowTransactionsWindow *win = new ShowTransactionsWindow(db, this);
    win->exec();
}

void MainWindow::openReports() {
    ReportsWindow *w = new ReportsWindow(user, this);
    w->exec();
}

void MainWindow::openDataIO() {
    DataWindow *w = new DataWindow(user, db, this);
    w->exec();
}


void MainWindow::openForecast() {
    ForecastWindow *fw = new ForecastWindow(db, this);
    fw->exec();
}

void MainWindow::saveAndExit() {
    saveUserData(user);
    QMessageBox::information(this, "Exit", "User data saved. Exiting application...");
    close();
}
