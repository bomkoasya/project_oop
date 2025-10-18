#include "gtest/gtest.h"
#include "../Importer.h"
#include <vector>
#include <ctime>

TEST(CSVImporterTest, ParseDateValid) {
    CSVImporter importer("");
    std::time_t result = importer.parseDate("2023-10-26");
    ASSERT_NE(result, 0);

    std::tm* ptm = std::gmtime(&result);
    EXPECT_EQ(ptm->tm_year, 123); // years since 1900
    EXPECT_EQ(ptm->tm_mon, 9); // months since January
    EXPECT_EQ(ptm->tm_mday, 26);
}

TEST(CSVImporterTest, ParseDateInvalid) {
    CSVImporter importer("");
    std::time_t result = importer.parseDate("invalid-date");
    EXPECT_EQ(result, 0);
}

TEST(CSVImporterTest, ImportValid) {
    CSVImporter importer("tests/valid_import.csv");
    auto result = importer.import();
    ASSERT_TRUE(result.is_success());
    auto transactions = result.get_value();
    ASSERT_EQ(transactions.size(), 2);
    EXPECT_EQ(transactions[0].description, "Test 1");
    EXPECT_DOUBLE_EQ(transactions[0].amount, 10.00);
    EXPECT_EQ(transactions[1].description, "Test 2");
    EXPECT_DOUBLE_EQ(transactions[1].amount, 20.50);
}

TEST(CSVImporterTest, ImportInvalidAmount) {
    CSVImporter importer("tests/invalid_amount.csv");
    auto result = importer.import();
    ASSERT_FALSE(result.is_success());
    EXPECT_EQ(result.get_error(), "Invalid amount format on line 2: invalid");
}

TEST(CSVImporterTest, ImportNonExistentFile) {
    CSVImporter importer("nonexistent.csv");
    auto result = importer.import();
    ASSERT_FALSE(result.is_success());
    EXPECT_EQ(result.get_error(), "Cannot open file: nonexistent.csv");
}