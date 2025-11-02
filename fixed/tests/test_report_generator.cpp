#include "gtest/gtest.h"
#include "../ReportGenerator.h"
#include "../Transaction.h"
#include <vector>
#include <fstream>
#include "../json.hpp"

class ReportGeneratorTest : public ::testing::Test {
protected:
    void SetUp() override {
        t1.amount = 100.0;
        t1.categoryId = "cat1";
        t1.accountId = "acc1";

        t2.amount = 50.0;
        t2.categoryId = "cat2";
        t2.accountId = "acc1";

        t3.amount = 200.0;
        t3.categoryId = "cat1";
        t3.accountId = "acc2";

        rg.addTransaction(t1);
        rg.addTransaction(t2);
        rg.addTransaction(t3);
    }

    ReportGenerator rg;
    Transaction t1, t2, t3;
};

TEST_F(ReportGeneratorTest, CalculateTotal) {
    EXPECT_DOUBLE_EQ(rg.calculateTotal(), 350.0);
}

TEST_F(ReportGeneratorTest, CalculateByCategory) {
    EXPECT_DOUBLE_EQ(rg.calculateByCategory("cat1"), 300.0);
    EXPECT_DOUBLE_EQ(rg.calculateByCategory("cat2"), 50.0);
    EXPECT_DOUBLE_EQ(rg.calculateByCategory("cat3"), 0.0);
}

TEST_F(ReportGeneratorTest, CalculateByAccount) {
    EXPECT_DOUBLE_EQ(rg.calculateByAccount("acc1"), 150.0);
    EXPECT_DOUBLE_EQ(rg.calculateByAccount("acc2"), 200.0);
    EXPECT_DOUBLE_EQ(rg.calculateByAccount("acc3"), 0.0);
}

TEST_F(ReportGeneratorTest, ExportToCSV) {
    const std::string filePath = "test_report.csv";
    rg.exportToCSV(filePath);

    std::ifstream file(filePath);
    ASSERT_TRUE(file.is_open());

    std::string line;
    std::getline(file, line); // Header
    EXPECT_EQ(line, "ID,Amount,Date,Description,Category,Account,Currency");

    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
    }
    EXPECT_EQ(lineCount, 3);
    file.close();
    std::remove(filePath.c_str());
}

TEST_F(ReportGeneratorTest, ExportToJSON) {
    const std::string filePath = "test_report.json";
    rg.exportToJSON(filePath);

    std::ifstream file(filePath);
    ASSERT_TRUE(file.is_open());

    nlohmann::json j;
    file >> j;

    EXPECT_EQ(j["reportTitle"], "Finance Report");
    EXPECT_EQ(j["totalAmount"], 350.0);
    ASSERT_EQ(j["transactions"].size(), 3);
    EXPECT_EQ(j["transactions"][0]["amount"], 100.0);

    file.close();
    std::remove(filePath.c_str());
}