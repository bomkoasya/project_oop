#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    static QTranslator translator;

    QString lang = QLocale::system().name();

    if (translator.load(":/translations/MyFinanceApp_" + lang + ".qm")) {
        app.installTranslator(&translator);
        qDebug() << "Loaded translation:" << lang;
    } else {
        qDebug() << "Failed to load translation for:" << lang;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
