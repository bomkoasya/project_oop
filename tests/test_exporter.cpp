#include "gtest/gtest.h"
#include "../Exporter.h"
#include "../Transaction.h"
#include <vector>
#include <fstream>
#include <string>

TEST(CSVExporterTest, ExportData) {
    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 10.00, 1672531200, "Test 1", "cat1"});
    txs.push_back(Transaction{"", 20.50, 1672617600, "Test 2", "cat2"});

    CSVExporter exporter("test_export.csv");
    ASSERT_TRUE(exporter.exportData(txs));

    std::ifstream file("test_export.csv");
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "1672531200,Test 1,10");
    std::getline(file, line);
    EXPECT_EQ(line, "1672617600,Test 2,20.5");
}

TEST(ExcelExporterTest, ExportData) {
    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 10.00, 1672531200, "Test 1", "cat1"});
    txs.push_back(Transaction{"", 20.50, 1672617600, "Test 2", "cat2"});

    ExcelExporter exporter("test_export.xlsx");
    ASSERT_TRUE(exporter.exportData(txs));

    std::ifstream file("test_export.xlsx");
    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "Date,Description,Amount");
    std::getline(file, line);
    EXPECT_EQ(line, "1672531200,Test 1,10");
    std::getline(file, line);
    EXPECT_EQ(line, "1672617600,Test 2,20.5");
}