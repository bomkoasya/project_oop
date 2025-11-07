/**
 * @file main.cpp
 * @brief Головний файл (entry point) програми MyFinanceApp.
 * @details Ініціалізує QApplication, налаштовує інтернаціоналізацію (QTranslator)
 * та запускає головне вікно MainWindow.
 */

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "gui/MainWindow.h"

/**
 * @brief Головна функція (entry point) програми.
 *
 * 1. Ініціалізує QApplication.
 * 2. Визначає системну мову (QLocale).
 * 3. Намагається завантажити відповідний .qm файл перекладу з ресурсів.
 * 4. Встановлює перекладач, якщо він успішно завантажений.
 * 5. Створює, показує головне вікно (MainWindow).
 * 6. Запускає цикл обробки подій програми.
 *
 * @param argc Кількість аргументів командного рядка.
 * @param argv Масив аргументів командного рядка.
 * @return Код виходу програми (0 у разі успіху).
 */
int main(int argc, char *argv[]) {
    // 1. Ініціалізація
    QApplication app(argc, argv);

    // 2-4. Налаштування перекладу
    // (Див. примітку нижче про дублювання коду)
    static QTranslator translator;
    QString lang = QLocale::system().name();

    if (translator.load(":/translations/MyFinanceApp_" + lang + ".qm")) {
        app.installTranslator(&translator);
        qDebug() << "Loaded translation:" << lang;
    } else {
        qDebug() << "Failed to load translation for:" << lang;
    }

    // 5. Створення головного вікна
    MainWindow window;
    window.show();

    // 6. Запуск програми
    return app.exec();
}
