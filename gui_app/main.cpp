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

    QApplication app(argc, argv);

    static QTranslator translator;
    QLocale locale = QLocale::system();
    QString lang = locale.name().split('_').first();

    QString translationPath = ":/i18n/MyFinanceApp_" + lang + ".qm";
    if (translator.load(translationPath)) {
        app.installTranslator(&translator);
        qDebug() << "Loaded translation for language:" << lang;
    } else {
        if (lang != "en") {
            translationPath = ":/i18n/MyFinanceApp_en.qm";
            if (translator.load(translationPath)) {
                app.installTranslator(&translator);
                qDebug() << "Fell back to English translation";
            } else {
                qDebug() << "No translation file found for:" << lang << "or English at path:" << translationPath;
            }
        } else {
            qDebug() << "No translation file found for:" << lang << "at path:" << translationPath;
        }
    }

    MainWindow window;
    window.show();

    return app.exec();
}
