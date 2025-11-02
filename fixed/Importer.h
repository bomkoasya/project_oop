#pragma once
#include <string>
#include <vector>
#include "Transaction.h"
#include "Result.h"

class Importer {
public:
    std::string sourceName;
    virtual ~Importer() = default;
    virtual Result<std::vector<Transaction>> import() = 0;
};

class CSVImporter : public Importer {
public:
    CSVImporter(const std::string& filePath);
    Result<std::vector<Transaction>> import() override;

private:
    std::string path;
    std::time_t parseDate(const std::string& dateStr) const;
};