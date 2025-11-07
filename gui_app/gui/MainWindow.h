#pragma once
#include <QMainWindow>
#include <QTranslator>
#include "core/Logic.h"

class QVBoxLayout;
class QLabel;
class QPushButton;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void openAddTransaction();
    void openShowTransactions();
    void openReports();
    void openDataIO();
    void openForecast();
    void saveAndExit();

    void switchToUkrainian();
    void switchToEnglish();

private:
    void retranslateUI();
    void installTranslator(const QString &lang);
    void loadUserData(User& user, Database& db);
    void saveUserData(User& user);
    QTranslator m_appTranslator;
    User user;
    Database db;
    QVBoxLayout *layout;
    QLabel *lblTitle;
    QPushButton *btnAdd;
    QPushButton *btnShow;
    QPushButton *btnReports;
    QPushButton *btnDataIO;
    QPushButton *btnForecast;
    QPushButton *btnSaveExit;
};
