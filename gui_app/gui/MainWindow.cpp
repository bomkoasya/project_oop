/**
 * @file MainWindow.cpp
 * @brief Реалізація головного вікна програми MyFinanceApp (MainWindow).
 */

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
#include <QMetaObject> // Додано для безпечного оновлення UI з фонових потоків
#include "ReportGenerator.h"

/**
 * @brief Конструктор MainWindow.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(400, 300);

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

        lblTitle->setText(tr("Завантаження даних..."));
        centralWidget()->setEnabled(false);

        loadUserDataAsync(user, db, [this]() {
            QMetaObject::invokeMethod(this, [this]() {
                centralWidget()->setEnabled(true);
                lblTitle->setText(tr("=== Personal Finance Manager ==="));

            });
        });

    } else {
        QMessageBox::information(this, tr("Info"), tr("New empty profile created."));
    }

    retranslateUI();
}

/**
 * @brief Оновлює всі тексти в інтерфейсі до поточної мови.
 */
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

/**
 * @brief Встановлює новий перекладач для програми.
 */
void MainWindow::installTranslator(const QString &lang)
{
    qApp->removeTranslator(&m_appTranslator);

    QString translationPath = ":/i18n/MyFinanceApp_" + lang + ".qm";
    if (m_appTranslator.load(translationPath)) {
        qApp->installTranslator(&m_appTranslator);
        qDebug() << "Switched to language:" << lang;
    } else {
        qWarning() << "Failed to load translation file for:" << lang << "at path:" << translationPath;
    }

    retranslateUI();
}

void MainWindow::switchToUkrainian() { installTranslator("uk"); }
void MainWindow::switchToEnglish() { installTranslator("en"); }

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

/**
 * @brief Слот: Зберігає дані користувача (асинхронно) та закриває програму.
 */
void MainWindow::saveAndExit() {
    centralWidget()->setEnabled(false);
    lblTitle->setText(tr("Збереження даних..."));

    saveUserDataAsync(user, [this]() {
        QMetaObject::invokeMethod(this, [this]() {
            QMessageBox::information(this, tr("Exit"), tr("User data saved. Exiting application..."));
            close();
        });
    });
}
