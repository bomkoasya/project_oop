#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

inline void to_json(json& j, const Transaction& t) {
    j = json{
        {"id", t.id},
        {"amount", t.amount},
        {"date", t.date},
        {"description", t.description},
        {"categoryId", t.categoryId},
        {"accountId", t.accountId},
        {"currency", t.currency},
        {"exchangeRate", t.exchangeRate},
        {"tags", t.tags},
        {"recurringRule", t.recurringRule},
        {"importedSource", t.importedSource},
        {"note", t.note},
        {"createdAt", t.createdAt},
        {"updatedAt", t.updatedAt}
    };
}

inline void from_json(const json& j, Transaction& t) {

    t.id = j.value("id", "");
    t.amount = j.value("amount", 0.0);
    t.date = j.value("date", 0);
    t.description = j.value("description", "");
    t.categoryId = j.value("categoryId", "");
    t.accountId = j.value("accountId", "");
    t.currency = j.value("currency", "USD");
    t.exchangeRate = j.value("exchangeRate", 1.0);
    t.tags = j.value("tags", std::vector<std::string>{});
    t.recurringRule = j.value("recurringRule", "");
    t.importedSource = j.value("importedSource", "");
    t.note = j.value("note", "");
    t.createdAt = j.value("createdAt", 0);
    t.updatedAt = j.value("updatedAt", 0);
}

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
