#include "gtest/gtest.h"
#include "../Budget.h"
#include "../Transaction.h"
#include <vector>

constexpr double kTolerance = 1e-9;
constexpr double kBudgetAmount = 100.0;
const std::string kCategory = "cat1";

struct BudgetTest : public ::testing::Test {
    Budget budget;
    void SetUp() override {
        budget.categoryId = kCategory;
        budget.amount = kBudgetAmount;
    }
};

TEST_F(BudgetTest, SuggestSavings_WhenOverBudget_ReturnsExcess) {
    std::vector<Transaction> txs = {
        {"", 50.0, 0, "", kCategory},
        {"", 70.0, 0, "", kCategory}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 20.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenUnderBudget_ReturnsZero) {
    std::vector<Transaction> txs = {
        {"", 30.0, 0, "", kCategory},
        {"", 40.0, 0, "", kCategory}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 0.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenAtBudget_ReturnsZero) {
    std::vector<Transaction> txs = {
        {"", 50.0, 0, "", kCategory},
        {"", 50.0, 0, "", kCategory}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 0.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenNoTransactions_ReturnsZero) {
    std::vector<Transaction> txs;
    EXPECT_NEAR(budget.suggestSavings(txs), 0.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenDifferentCategory_ReturnsZero) {
    std::vector<Transaction> txs = {
        {"", 150.0, 0, "", "cat2"}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 0.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenRefundTransaction_AdjustsSpendingDown) {
    std::vector<Transaction> txs = {
        {"", 120.0, 0, "", kCategory},
        {"", -20.0, 0, "", kCategory}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 0.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenZeroBudget_ReturnsFullSpending) {
    budget.amount = 0.0;
    std::vector<Transaction> txs = {
        {"", 50.0, 0, "", kCategory}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 50.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenNegativeBudget_ReturnsIncreasedExcess) {
    budget.amount = -50.0;
    std::vector<Transaction> txs = {
        {"", 50.0, 0, "", kCategory}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 100.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenMixedCategories_OnlyCountsMatching) {
    std::vector<Transaction> txs = {
        {"", 70.0, 0, "", kCategory},
        {"", 200.0, 0, "", "other"},
        {"", 50.0, 0, "", kCategory}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 20.0, kTolerance);
}

TEST_F(BudgetTest, SuggestSavings_WhenLargeAmounts_DoesNotOverflow) {
    budget.amount = 1e9;
    std::vector<Transaction> txs = {
        {"", 5e8, 0, "", kCategory},
        {"", 6e8, 0, "", kCategory}
    };
    EXPECT_NEAR(budget.suggestSavings(txs), 1e8, kTolerance);
}
