#include "Importer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>

CSVImporter::CSVImporter(const std::string& filePath) : path(filePath) {}

std::time_t CSVImporter::parseDate(const std::string& dateStr) const {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d"); // формат CSV: "YYYY-MM-DD"
    if (ss.fail()) {
        std::cerr << "Failed to parse date: " << dateStr << "\n";
        return 0;
    }
    return std::mktime(&tm);
}

std::vector<Transaction> CSVImporter::import() {
    std::vector<Transaction> transactions;

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << path << "\n";
        return transactions;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string dateStr, desc, amountStr;

        if (std::getline(ss, dateStr, ',') &&
            std::getline(ss, desc, ',') &&
            std::getline(ss, amountStr)) {

            Transaction t;
            t.date = parseDate(dateStr);
            t.description = desc;
            t.amount = std::stod(amountStr);

            transactions.push_back(t);
            }
    }

    return transactions;
}
