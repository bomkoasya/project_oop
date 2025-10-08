#pragma once
#include <string>
#include <vector>
#include "Transaction.h"

class Importer {
public:
    std::string sourceName;
    virtual ~Importer() = default;
    virtual std::vector<Transaction> import() = 0; // dynamic polymorphism
};

class CSVImporter : public Importer {
public:
    CSVImporter(const std::string& filePath);
    std::vector<Transaction> import() override;

private:
    std::string path;

    std::time_t parseDate(const std::string& dateStr) const;
};
