#include "MainWindow.h"
#include "Logic.h"
#include "gui/TransactionsWindow.h"
#include "gui/ReportsWindow.h"
#include "gui/DataWindow.h"
#include "gui/ForecastWindow.h"
#include "gui/ShowTransactionsWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLocale>
#include <QDebug>
#include "Logic.h"
#include "ReportGenerator.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(400, 300);

    const QLocale locale = QLocale::system();

    if (m_appTranslator.load(locale, "MyFinanceApp", "_", ":/translations")) {
        qApp->installTranslator(&m_appTranslator);
    } else {
        qWarning() << "Could not auto-load app translation for" << locale.name();
        if (m_appTranslator.load(":/translations/MyFinanceApp_en.qm")) {
            qApp->installTranslator(&m_appTranslator);
        }
    }

    QMenu *menuFile = menuBar()->addMenu("");
    QMenu *menuLang = menuBar()->addMenu("");

    QAction *actExit = new QAction(this);
    QAction *actUkr = new QAction(this);
    QAction *actEng = new QAction(this);

    menuFile->addAction(actExit);
    menuLang->addAction(actUkr);
    menuLang->addAction(actEng);

    connect(actExit, &QAction::triggered, this, &MainWindow::saveAndExit);
    connect(actUkr, &QAction::triggered, this, &MainWindow::switchToUkrainian);
    connect(actEng, &QAction::triggered, this, &MainWindow::switchToEnglish);

    QWidget *central = new QWidget(this);
    layout = new QVBoxLayout(central);

    lblTitle = new QLabel("", this);
    lblTitle->setAlignment(Qt::AlignCenter);

    btnAdd = new QPushButton(this);
    btnShow = new QPushButton(this);
    btnReports = new QPushButton(this);
    btnDataIO = new QPushButton(this);
    btnForecast = new QPushButton(this);
    btnSaveExit = new QPushButton(this);

    layout->addWidget(lblTitle);
    layout->addSpacing(10);
    layout->addWidget(btnAdd);
    layout->addWidget(btnShow);
    layout->addWidget(btnReports);
    layout->addWidget(btnDataIO);
    layout->addWidget(btnForecast);
    layout->addSpacing(20);
    layout->addWidget(btnSaveExit);

    setCentralWidget(central);

    connect(btnAdd,     &QPushButton::clicked, this, &MainWindow::openAddTransaction);
    connect(btnShow,    &QPushButton::clicked, this, &MainWindow::openShowTransactions);
    connect(btnReports, &QPushButton::clicked, this, &MainWindow::openReports);
    connect(btnDataIO,  &QPushButton::clicked, this, &MainWindow::openDataIO);
    connect(btnForecast,&QPushButton::clicked, this, &MainWindow::openForecast);
    connect(btnSaveExit,&QPushButton::clicked, this, &MainWindow::saveAndExit);

    bool ok;
    QString username = QInputDialog::getText(this, tr("User Login"),
                                             tr("Enter your username (profile ID):"),
                                             QLineEdit::Normal, "", &ok);
    if (ok && !username.isEmpty()) {
        user.id = username.toStdString();
        user.name = user.id;
        loadUserData(user, db);
    } else {
        QMessageBox::information(this, tr("Info"), tr("New empty profile created."));
    }

    retranslateUI();
}

void MainWindow::retranslateUI()
{
    setWindowTitle(tr("Personal Finance Manager"));

    menuBar()->actions()[0]->setText(tr("File"));
    menuBar()->actions()[1]->setText(tr("Language"));

    menuBar()->actions()[0]->menu()->actions().at(0)->setText(tr("Exit"));
    menuBar()->actions()[1]->menu()->actions().at(0)->setText(tr("Ukrainian"));
    menuBar()->actions()[1]->menu()->actions().at(1)->setText(tr("English"));

    lblTitle->setText(tr("=== Personal Finance Manager ==="));
    btnAdd->setText(tr("Add Transaction"));
    btnShow->setText(tr("Show All Transactions"));
    btnReports->setText(tr("Generate Reports"));
    btnDataIO->setText(tr("Data Import / Export"));
    btnForecast->setText(tr("Forecast"));
    btnSaveExit->setText(tr("Save and Exit"));
}

void MainWindow::installTranslator(const QString &lang)
{
    qApp->removeTranslator(&m_appTranslator);

    if (m_appTranslator.load(":/translations/MyFinanceApp_" + lang + ".qm")) {
        qApp->installTranslator(&m_appTranslator);
    } else {
        qWarning() << "Failed to load translation file for: " << lang;
    }

    retranslateUI();
}

void MainWindow::switchToUkrainian()
{
    installTranslator("uk");
}

void MainWindow::switchToEnglish()
{
    installTranslator("en");
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
    QMessageBox::information(this, tr("Exit"), tr("User data saved. Exiting application..."));
    close();
}



void MainWindow::loadUserData(User& userToLoad, Database& dbToLoad) {

    qDebug() << "User data loaded (or not implemented) for" << QString::fromStdString(userToLoad.id);
}

void MainWindow::saveUserData(User& userToSave) {

    qDebug() << "User data saved (or not implemented) for" << QString::fromStdString(userToSave.id);
}
