/**
 * @file MainWindow.cpp
 * @brief Реалізація головного вікна програми MyFinanceApp (MainWindow).
 */

#include "MainWindow.h"
#include "Logic.h" // Містить saveUserData/loadUserData
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
#include "ReportGenerator.h"

/**
 * @brief Конструктор MainWindow.
 *
 * Виконує повну ініціалізацію головного вікна:
 * 1. Налаштовує розмір.
 * 2. Завантажує файли перекладу (QTranslator) на основі системної мови.
 * 3. Створює верхнє меню (MenuBar) з опціями "Файл" та "Мова".
 * 4. Створює центральний віджет з усіма кнопками головного меню (Додати, Показати тощо).
 * 5. Встановлює всі з'єднання (connect) для меню та кнопок.
 * 6. Показує QInputDialog для отримання імені користувача.
 * 7. Завантажує профіль користувача (loadUserData) або створює новий.
 * 8. Викликає retranslateUI() для встановлення початкових текстів.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(400, 300);

    // --- 2. Налаштування перекладу ---
    const QLocale locale = QLocale::system();
    // Спроба завантажити системну мову
    if (m_appTranslator.load(locale, "MyFinanceApp", "_", ":/translations")) {
        qApp->installTranslator(&m_appTranslator);
    } else {
        qWarning() << "Could not auto-load app translation for" << locale.name();
        // Фолбек на англійську, якщо системна не знайдена
        if (m_appTranslator.load(":/translations/MyFinanceApp_en.qm")) {
            qApp->installTranslator(&m_appTranslator);
        }
    }

    // --- 3. Створення MenuBar ---
    QMenu *menuFile = menuBar()->addMenu(""); // Текст буде встановлено в retranslateUI
    QMenu *menuLang = menuBar()->addMenu(""); // Текст буде встановлено в retranslateUI

    QAction *actExit = new QAction(this);
    QAction *actUkr = new QAction(this);
    QAction *actEng = new QAction(this);

    menuFile->addAction(actExit);
    menuLang->addAction(actUkr);
    menuLang->addAction(actEng);

    connect(actExit, &QAction::triggered, this, &MainWindow::saveAndExit);
    connect(actUkr, &QAction::triggered, this, &MainWindow::switchToUkrainian);
    connect(actEng, &QAction::triggered, this, &MainWindow::switchToEnglish);

    // --- 4. Створення центрального віджета та кнопок ---
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

    // --- 5. З'єднання кнопок ---
    connect(btnAdd,     &QPushButton::clicked, this, &MainWindow::openAddTransaction);
    connect(btnShow,    &QPushButton::clicked, this, &MainWindow::openShowTransactions);
    connect(btnReports, &QPushButton::clicked, this, &MainWindow::openReports);
    connect(btnDataIO,  &QPushButton::clicked, this, &MainWindow::openDataIO);
    connect(btnForecast,&QPushButton::clicked, this, &MainWindow::openForecast);
    connect(btnSaveExit,&QPushButton::clicked, this, &MainWindow::saveAndExit);

    // --- 6. Логін користувача ---
    bool ok;
    QString username = QInputDialog::getText(this, tr("User Login"),
                                             tr("Enter your username (profile ID):"),
                                             QLineEdit::Normal, "", &ok);

    // --- 7. Завантаження профілю ---
    if (ok && !username.isEmpty()) {
        user.id = username.toStdString();
        user.name = user.id; // Припускаємо, що ім'я та ID однакові
        loadUserData(user, db); // Завантаження даних з БД
    } else {
        QMessageBox::information(this, tr("Info"), tr("New empty profile created."));
        // user об'єкт залишається пустим
    }

    // --- 8. Встановлення текстів ---
    retranslateUI();
}

/**
 * @brief Оновлює всі тексти в інтерфейсі до поточної мови.
 *
 * Ця функція викликається один раз у конструкторі та щоразу
 * при зміні мови через installTranslator.
 */
void MainWindow::retranslateUI()
{
    setWindowTitle(tr("Personal Finance Manager"));

    // Оновлення MenuBar
    menuBar()->actions()[0]->setText(tr("File"));
    menuBar()->actions()[1]->setText(tr("Language"));

    menuBar()->actions()[0]->menu()->actions().at(0)->setText(tr("Exit"));
    menuBar()->actions()[1]->menu()->actions().at(0)->setText(tr("Ukrainian"));
    menuBar()->actions()[1]->menu()->actions().at(1)->setText(tr("English"));

    // Оновлення кнопок
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
 *
 * Видаляє старий перекладач, завантажує новий файл .qm з ресурсів
 * на основі коду мови (напр., "uk" або "en"), і викликає retranslateUI()
 * для миттєвого оновлення інтерфейсу.
 *
 * @param lang Код мови ("uk", "en" тощо), що відповідає назві файлу .qm.
 */
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

/**
 * @brief Слот для перемикання на українську мову.
 */
void MainWindow::switchToUkrainian()
{
    installTranslator("uk");
}

/**
 * @brief Слот для перемикання на англійську мову.
 */
void MainWindow::switchToEnglish()
{
    installTranslator("en");
}


// --- Слоти для відкриття діалогових вікон ---

/**
 * @brief Слот: Відкриває вікно додавання транзакцій.
 */
void MainWindow::openAddTransaction() {
    TransactionsWindow *w = new TransactionsWindow(user, db, this);
    w->exec(); // .exec() відкриває вікно модально
}

/**
 * @brief Слот: Відкриває вікно перегляду всіх транзакцій.
 */
void MainWindow::openShowTransactions() {
    ShowTransactionsWindow *win = new ShowTransactionsWindow(db, this);
    win->exec();
}

/**
 * @brief Слот: Відкриває вікно генерації звітів.
 */
void MainWindow::openReports() {
    ReportsWindow *w = new ReportsWindow(user, this);
    w->exec();
}

/**
 * @brief Слот: Відкриває вікно імпорту/експорту даних.
 */
void MainWindow::openDataIO() {
    DataWindow *w = new DataWindow(user, db, this);
    w->exec();
}

/**
 * @brief Слот: Відкриває вікно прогнозування.
 */
void MainWindow::openForecast() {
    ForecastWindow *fw = new ForecastWindow(db, this);
    fw->exec();
}

/**
 * @brief Слот: Зберігає дані користувача та закриває програму.
 */
void MainWindow::saveAndExit() {
    saveUserData(user);
    QMessageBox::information(this, tr("Exit"), tr("User data saved. Exiting application..."));
    close();
}


// --- Функції завантаження/збереження (заглушки) ---

/**
 * @brief Завантажує дані користувача з БД. (Зараз - заглушка).
 * @param userToLoad Об'єкт користувача, який буде заповнено.
 * @param dbToLoad База даних, з якої читаються дані.
 */
void MainWindow::loadUserData(User& userToLoad, Database& dbToLoad) {
    // TODO: Тут має бути реальна логіка завантаження з dbToLoad
    qDebug() << "User data loaded (or not implemented) for" << QString::fromStdString(userToLoad.id);
}

/**
 * @brief Зберігає дані користувача в БД. (Зараз - заглушка).
 * @param userToSave Об'єкт користувача, дані якого зберігаються.
 */
void MainWindow::saveUserData(User& userToSave) {
    // TODO: Тут має бути реальна логіка збереження в db
    qDebug() << "User data saved (or not implemented) for" << QString::fromStdString(userToSave.id);
}
