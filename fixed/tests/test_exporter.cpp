#include "gtest/gtest.h"
#include "../Exporter.h"
#include "../Transaction.h"
#include <vector>
#include <fstream>
#include <string>
#include <cstdio>

constexpr double kAmount1 = 10.0;
constexpr double kAmount2 = 20.5;
constexpr long long kTime1 = 1672531200;
constexpr long long kTime2 = 1672617600;
constexpr double kTolerance = 1e-9;

struct ExporterTest : public ::testing::Test {
    std::vector<Transaction> txs;

    void SetUp() override {
        txs = {
            {"", kAmount1, kTime1, "Test 1", "cat1"},
            {"", kAmount2, kTime2, "Test 2", "cat2"}
        };
    }

    void TearDown() override {
        std::remove("test_export.csv");
        std::remove("test_export.xlsx");
    }

    bool fileExists(const std::string& path) {
        std::ifstream f(path);
        return f.good();
    }
};

TEST_F(ExporterTest, GIVEN_ValidTransactions_WHEN_ExportedAsCSV_THEN_FileContainsExpectedLines) {
    CSVExporter exporter("test_export.csv");
    ASSERT_TRUE(exporter.exportData(txs));

    ASSERT_TRUE(fileExists("test_export.csv")) << "CSV file was not created";

    std::ifstream file("test_export.csv");
    ASSERT_TRUE(file.is_open()) << "Failed to open CSV file";

    std::string line;

    std::getline(file, line);
    EXPECT_TRUE(line.find("1672531200") != std::string::npos);
    EXPECT_TRUE(line.find("Test 1") != std::string::npos);
    EXPECT_TRUE(line.find("10") != std::string::npos);

    std::getline(file, line);
    EXPECT_TRUE(line.find("1672617600") != std::string::npos);
    EXPECT_TRUE(line.find("Test 2") != std::string::npos);
    EXPECT_TRUE(line.find("20.5") != std::string::npos);
}

TEST_F(ExporterTest, GIVEN_ValidTransactions_WHEN_ExportedAsExcel_THEN_FileContainsHeaderAndData) {
    ExcelExporter exporter("test_export.xlsx");
    ASSERT_TRUE(exporter.exportData(txs));

    ASSERT_TRUE(fileExists("test_export.xlsx")) << "Excel file was not created";

    std::ifstream file("test_export.xlsx");
    ASSERT_TRUE(file.is_open()) << "Failed to open Excel file";

    std::string line;
    std::getline(file, line);
    EXPECT_EQ(line, "Date,Description,Amount");

    std::getline(file, line);
    EXPECT_TRUE(line.find("1672531200") != std::string::npos);
    EXPECT_TRUE(line.find("Test 1") != std::string::npos);
    EXPECT_TRUE(line.find("10") != std::string::npos);

    std::getline(file, line);
    EXPECT_TRUE(line.find("1672617600") != std::string::npos);
    EXPECT_TRUE(line.find("Test 2") != std::string::npos);
    EXPECT_TRUE(line.find("20.5") != std::string::npos);
}

TEST_F(ExporterTest, GIVEN_NoTransactions_WHEN_Exported_THEN_FileIsEmptyOrHeaderOnly) {
    CSVExporter exporter("test_export.csv");
    std::vector<Transaction> emptyTxs;

    ASSERT_TRUE(exporter.exportData(emptyTxs));

    std::ifstream file("test_export.csv");
    ASSERT_TRUE(file.is_open());
    std::string content((std::istreambuf_iterator<char>(file)), {});
    EXPECT_TRUE(content.empty() || content.find("Date") != std::string::npos);
}

TEST_F(ExporterTest, GIVEN_NegativeAmounts_WHEN_Exported_THEN_ValuesWrittenCorrectly) {
    txs = { {"", -5.75, kTime1, "Refund", "cat1"} };

    CSVExporter exporter("test_export.csv");
    ASSERT_TRUE(exporter.exportData(txs));

    std::ifstream file("test_export.csv");
    std::string line;
    std::getline(file, line);
    EXPECT_TRUE(line.find("-5.75") != std::string::npos);
}

TEST_F(ExporterTest, GIVEN_InvalidFilePath_WHEN_Exported_THEN_ReturnsFalse) {
    CSVExporter exporter("/invalid_path/test.csv");
    EXPECT_FALSE(exporter.exportData(txs));
}

TEST_F(ExporterTest, GIVEN_DescriptionWithComma_WHEN_ExportedToCSV_THEN_CommaIsHandled) {
    txs = { {"", 12.3, kTime1, "Lunch, with friends", "cat1"} };
    CSVExporter exporter("test_export.csv");
    ASSERT_TRUE(exporter.exportData(txs));

    std::ifstream file("test_export.csv");
    std::string line;
    std::getline(file, line);
    EXPECT_TRUE(line.find("\"Lunch, with friends\"") != std::string::npos);
}
