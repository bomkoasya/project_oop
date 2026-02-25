#pragma once
#include "User.h"
#include "Database.h"
#include <functional>

/**
 * @file Logic.h
 * @brief Оголошення основних функцій бізнес-логіки програми.
 *
 * Цей файл містить набір вільних функцій, які діють як "контролер"
 * або "сервісний шар", пов'язуючи об'єкти даних (User, Database)
 * з діями, які викликає GUI.
 */

/**
 * @brief Завантажує дані користувача з бази даних при старті програми.
 * @param user Об'єкт користувача (вихідний параметр), який буде заповнено даними.
 * @param db Об'єкт бази даних, з якої відбувається читання.
 */
void loadUserData(User& user, Database& db);

/**
 * @brief Зберігає дані користувача в базу даних (наприклад, при виході).
 * @param user Об'єкт користувача (вхідний параметр, const), дані якого потрібно зберегти.
 */
void saveUserData(const User& user);

void loadUserDataAsync(User& user, Database& db, std::function<void()> onComplete);
void saveUserDataAsync(User user, std::function<void()> onComplete);


