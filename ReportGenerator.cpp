#include "ReportGenerator.h"
#include <fstream>
#include <map>
#include "json.hpp"

using json = nlohmann::json;

void ReportGenerator::addTransaction(const Transaction& t) {
    transactions.push_back(t);
}

double ReportGenerator::calculateTotal() const {
    double sum = 0.0;
    for (const auto& t : transactions) {
        sum += t.amount;
    }
    return sum;
}

double ReportGenerator::calculateByCategory(const std::string& categoryId) const {
    double sum = 0.0;
    for (const auto& t : transactions) {
        if (t.categoryId == categoryId) {
            sum += t.amount;
        }
    }
    return sum;
}

double ReportGenerator::calculateByAccount(const std::string& accountId) const {
    double sum = 0.0;
    for (const auto& t : transactions) {
        if (t.accountId == accountId) {
            sum += t.amount;
        }
    }
    return sum;
}

void ReportGenerator::exportToCSV(const std::string& filePath) const {
    std::ofstream file(filePath);
    file << "ID,Amount,Date,Description,Category,Account,Currency\n";
    for (const auto& t : transactions) {
        file << t.id << "," << t.amount << "," << t.date << ","
             << t.description << "," << t.categoryId << ","
             << t.accountId << "," << t.currency << "\n";
    }
}

void ReportGenerator::exportToJSON(const std::string& filePath) const {
    json j;
    for (const auto& t : transactions) {
        j["transactions"].push_back({
            {"id", t.id},
            {"amount", t.amount},
            {"date", t.date},
            {"description", t.description},
            {"categoryId", t.categoryId},
            {"accountId", t.accountId},
            {"currency", t.currency}
        });
    }
    std::ofstream file(filePath);
    file << j.dump(4);
}

std::map<std::string, double> ReportGenerator::preparePieChartData() const {
    std::map<std::string, double> categoryTotals;
    for (const auto& t : transactions) {
        categoryTotals[t.categoryId] += t.amount;
    }
    return categoryTotals;
}
