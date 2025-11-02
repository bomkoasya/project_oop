#include "Importer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "Result.h"

CSVImporter::CSVImporter(const std::string& filePath) : path(filePath) {}

std::time_t CSVImporter::parseDate(const std::string& dateStr) const {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    if (ss.fail()) {
        std::cerr << "Failed to parse date: " << dateStr << "\n";
        return 0;
    }
    return std::mktime(&tm);
}

Result<std::vector<Transaction>> CSVImporter::import() {
    std::vector<Transaction> transactions;
    std::ifstream file(path);

    if (!file.is_open()) {
        return Result<std::vector<Transaction>>::error("Cannot open file: " + path);
    }

    std::string line;
    int line_number = 0;
    while (std::getline(file, line)) {
        line_number++;
        std::istringstream ss(line);
        std::string dateStr, desc, amountStr;

        if (std::getline(ss, dateStr, ',') &&
            std::getline(ss, desc, ',') &&
            std::getline(ss, amountStr)) {

            Transaction t;
            t.date = parseDate(dateStr);
            t.description = desc;

            try {
                t.amount = std::stod(amountStr);
            } catch (const std::exception& e) {
                return Result<std::vector<Transaction>>::error(
                    "Invalid amount format on line " + std::to_string(line_number) + ": " + amountStr
                );
            }

            transactions.push_back(t);
        }
    }

    return Result<std::vector<Transaction>>::success(transactions);
}