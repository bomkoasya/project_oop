#include "Exporter.h"
#include <fstream>
#include <iostream>

CSVExporter::CSVExporter(const std::string& path) : filePath(path) {}

bool CSVExporter::exportData(const std::vector<Transaction>& transactions) {
    std::ofstream file(filePath);
    if (!file.is_open()) { std::cerr << "Cannot open file\n"; return false; }
    for (const auto& t : transactions)
        file << t.date << "," << t.description << "," << t.amount << "\n";
    return true;
}

ExcelExporter::ExcelExporter(const std::string& path) : filePath(path) {}

bool ExcelExporter::exportData(const std::vector<Transaction>& transactions) {
    std::ofstream file(filePath);
    if (!file.is_open()) { std::cerr << "Cannot open file\n"; return false; }
    file << "Date,Description,Amount\n";
    for (const auto& t : transactions)
        file << t.date << "," << t.description << "," << t.amount << "\n";
    return true;
}
