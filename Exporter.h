#pragma once
#include <vector>
#include <string>
#include "Transaction.h"

class CSVExporter {
public:
    CSVExporter(const std::string& filePath);
    bool exportData(const std::vector<Transaction>& transactions);

private:
    std::string filePath;
};

class ExcelExporter {
public:
    ExcelExporter(const std::string& filePath);
    bool exportData(const std::vector<Transaction>& transactions);

private:
    std::string filePath;
};
