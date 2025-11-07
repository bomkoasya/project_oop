#pragma once

#include <QMainWindow>
#include <QTranslator>
#include "core/Logic.h"

class QVBoxLayout;
class QLabel;
class QPushButton;
class QMenu;

/**
 * @file MainWindow.h
 * @brief Оголошення головного вікна програми MyFinanceApp.
 */

/**
 * @class MainWindow
 * @brief Головне вікно програми (GUI).
 *
 * Цей клас є центральною точкою входу для користувача. Він містить
 * головне меню для доступу до всіх основних функцій: додавання транзакцій,
 * перегляду звітів, імпорту/експорту, прогнозування та налаштування мови.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класу MainWindow.
     * @param parent Вказівник на батьківський віджет (зазвичай 0 або nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Відкриває діалогове вікно для додавання нової транзакції.
     */
    void openAddTransaction();

    /**
     * @brief Відкриває вікно для перегляду списку всіх транзакцій.
     */
    void openShowTransactions();

    /**
     * @brief Відкриває вікно генерації та перегляду звітів.
     */
    void openReports();

    /**
     * @brief Відкриває вікно імпорту/експорту даних (напр., CSV).
     */
    void openDataIO();

    /**
     * @brief Відкриває вікно фінансового прогнозування.
     */
    void openForecast();

    /**
     * @brief Зберігає дані користувача та закриває програму.
     */
    void saveAndExit();

    /**
     * @brief Перемикає мову інтерфейсу на українську.
     */
    void switchToUkrainian();

    /**
     * @brief Перемикає мову інтерфейсу на англійську.
     */
    void switchToEnglish();

private:
    /**
     * @brief Оновлює тексти всіх елементів інтерфейсу відповідно до обраної мови.
     *
     * Викликається автоматично після встановлення нового перекладача.
     */
    void retranslateUI();

    /**
     * @brief Встановлює та активує файл перекладу для програми.
     * @param lang Код мови (наприклад, "uk" або "en").
     */
    void installTranslator(const QString &lang);

    /**
     * @brief Завантажує дані користувача з бази даних.
     * @param user Об'єкт користувача для заповнення.
     * @param db База даних, з якої читаються дані.
     */
    void loadUserData(User& user, Database& db);

    /**
     * @brief Зберігає поточні дані користувача в базу даних.
     * @param user Об'єкт користувача, дані якого потрібно зберегти.
     */
    void saveUserData(User& user);

    QTranslator m_appTranslator; ///< Об'єкт, що керує завантаженням файлів перекладу (.qm).
    User user;                   ///< Об'єкт, що зберігає дані поточного користувача.
    Database db;                 ///< Об'єкт для взаємодії з базою даних (завантаження, збереження).

    QVBoxLayout *layout;         ///< Головний менеджер компонування для вікна.
    QLabel *lblTitle;            ///< Заголовок вікна.
    QPushButton *btnAdd;         ///< Кнопка "Додати транзакцію".
    QPushButton *btnShow;        ///< Кнопка "Показати транзакції".
    QPushButton *btnReports;     ///< Кнопка "Звіти".
    QPushButton *btnDataIO;      ///< Кнопка "Імпорт/Експорт".
    QPushButton *btnForecast;    ///< Кнопка "Прогноз".
    QPushButton *btnSaveExit;    ///< Кнопка "Зберегти та вийти".
};
