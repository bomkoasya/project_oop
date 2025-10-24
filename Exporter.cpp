#include "Exporter.h"
#include <fstream>
#include <iostream>

static std::string escapeCSVField(const std::string& field) {
    std::string result = field;

    if (result.find(',') != std::string::npos ||
        result.find('"') != std::string::npos ||
        result.find('\n') != std::string::npos) {

        size_t pos = 0;
        while ((pos = result.find('"', pos)) != std::string::npos) {
            result.insert(pos, "\"");
            pos += 2;
        }

        result = "\"" + result + "\"";
        }
    return result;
}

CSVExporter::CSVExporter(const std::string& path) : filePath(path) {}

bool CSVExporter::exportData(const std::vector<Transaction>& transactions) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file\n";
        return false;
    }

    for (const auto& t : transactions) {
        file << escapeCSVField(std::to_string(t.date)) << ","
             << escapeCSVField(t.description) << ","
             << escapeCSVField(std::to_string(t.amount)) << "\n";
    }

    return true;
}

ExcelExporter::ExcelExporter(const std::string& path) : filePath(path) {}

bool ExcelExporter::exportData(const std::vector<Transaction>& transactions) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file\n";
        return false;
    }

    file << "Date,Description,Amount\n";
    for (const auto& t : transactions) {
        file << escapeCSVField(std::to_string(t.date)) << ","
             << escapeCSVField(t.description) << ","
             << escapeCSVField(std::to_string(t.amount)) << "\n";
    }

    return true;
}
