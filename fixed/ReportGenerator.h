#pragma once
#include <vector>
#include <string>
#include <map>
#include "Transaction.h"

class ReportGenerator {
public:
    std::string reportTitle;
    std::vector<Transaction> transactions;

    ReportGenerator(const std::string& title = "Finance Report")
        : reportTitle(title) {}

    void addTransaction(const Transaction& t);
    double calculateTotal() const;
    double calculateByCategory(const std::string& categoryId) const;
    double calculateByAccount(const std::string& accountId) const;
    void exportToCSV(const std::string& filePath) const;
    void exportToJSON(const std::string& filePath) const;
    std::map<std::string, double> preparePieChartData() const;
};
