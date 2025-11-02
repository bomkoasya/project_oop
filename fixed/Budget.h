#pragma once
#include <string>
#include <vector>
#include "Transaction.h"


class Budget {
public:
    std::string id;
    std::string userId;
    std::string categoryId;
    double amount = 0.0; // period amount
    std::string period; // monthly | weekly | yearly
    std::vector<std::string> rules;
    std::string note;


    void applyBudgetRules(const std::vector<Transaction>& txs);
    double suggestSavings(const std::vector<Transaction>& txs) const;
};


inline void Budget::applyBudgetRules(const std::vector<Transaction>& txs) {

    (void)txs;
}
inline double Budget::suggestSavings(const std::vector<Transaction>& txs) const {
    double spent = 0.0;
    for (const auto &t : txs) if (t.categoryId == categoryId) spent += t.amount;
    double over = spent - amount;
    return over > 0.0 ? over : 0.0;
}