#pragma once
#include <string>
#include <vector>
#include <cmath>
#include "Transaction.h"


class Account {
public:
    std::string id;
    std::string name;
    double balance = 0.0;
    std::string currency = "USD";
    std::string userId;
    std::time_t createdAt = 0;


    virtual ~Account() = default;


    virtual bool reconcileAccount(const std::vector<Transaction>& transactions);
    virtual void persist();
    virtual std::string type() const { return "Account"; }
};


inline bool Account::reconcileAccount(const std::vector<Transaction>& transactions) {
    double computed = 0.0;
    for (const auto &t : transactions) {
        if (t.accountId == id) computed += t.amount;
    }

    return std::abs(computed - balance) < 0.01;
}
inline void Account::persist() { }


class CheckingAccount : public Account { public: std::string type() const override { return "CheckingAccount"; } };
class CreditAccount : public Account { public: double creditLimit = 0.0; std::string type() const override { return "CreditAccount"; } };
class SavingsAccount : public Account { public: double interestRate = 0.0; std::string type() const override { return "SavingsAccount"; } };
