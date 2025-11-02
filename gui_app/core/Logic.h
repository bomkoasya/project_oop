#pragma once
#include "User.h"
#include "Database.h"

void loadUserData(User& user, Database& db);
void saveUserData(const User& user);
void addTransaction(User& user, Database& db);
void showTransactions(const User& user);
void handleReports(const User& user);
void handleDataIO(User& user, Database& db);
void handleForecasting(const Database& db);
