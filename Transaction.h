#pragma once
#include <string>
#include <vector>
#include <ctime>


class Transaction {
public:
    std::string id;
    double amount = 0.0;
    std::time_t date = 0;
    std::string description;
    std::string categoryId;
    std::string accountId;
    std::string currency = "USD";
    double exchangeRate = 1.0;
    std::vector<std::string> tags;
    std::string recurringRule;
    std::string importedSource;
    std::string note;
    std::time_t createdAt = 0;
    std::time_t updatedAt = 0;


    void addTransaction();
    std::vector<Transaction> splitTransaction(const std::vector<std::pair<double, std::string>>& parts);
    void tagTransaction(const std::string& tag);
    void undoTransaction();
};


inline void Transaction::addTransaction() { createdAt = std::time(nullptr); }
inline std::vector<Transaction> Transaction::splitTransaction(const std::vector<std::pair<double, std::string>>& parts) {
    std::vector<Transaction> out;
    for (const auto &p : parts) {
        Transaction t = *this;
        t.amount = p.first;
        t.categoryId = p.second;
        out.push_back(t);
    }
    return out;
}
inline void Transaction::tagTransaction(const std::string& tag) { tags.push_back(tag); }
inline void Transaction::undoTransaction() { /* placeholder */ }