#include "gtest/gtest.h"
#include "../Budget.h"
#include "../Transaction.h"
#include <vector>

TEST(BudgetTest, SuggestSavingsOverBudget) {
    Budget budget;
    budget.categoryId = "cat1";
    budget.amount = 100.0;

    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 50.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 70.0, 0, "", "cat1"});

    EXPECT_DOUBLE_EQ(budget.suggestSavings(txs), 20.0);
}

TEST(BudgetTest, SuggestSavingsUnderBudget) {
    Budget budget;
    budget.categoryId = "cat1";
    budget.amount = 100.0;

    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 30.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 40.0, 0, "", "cat1"});

    EXPECT_DOUBLE_EQ(budget.suggestSavings(txs), 0.0);
}

TEST(BudgetTest, SuggestSavingsAtBudget) {
    Budget budget;
    budget.categoryId = "cat1";
    budget.amount = 100.0;

    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 50.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 50.0, 0, "", "cat1"});

    EXPECT_DOUBLE_EQ(budget.suggestSavings(txs), 0.0);
}

TEST(BudgetTest, SuggestSavingsNoTransactions) {
    Budget budget;
    budget.categoryId = "cat1";
    budget.amount = 100.0;

    std::vector<Transaction> txs;

    EXPECT_DOUBLE_EQ(budget.suggestSavings(txs), 0.0);
}

TEST(BudgetTest, SuggestSavingsDifferentCategory) {
    Budget budget;
    budget.categoryId = "cat1";
    budget.amount = 100.0;

    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 150.0, 0, "", "cat2"});

    EXPECT_DOUBLE_EQ(budget.suggestSavings(txs), 0.0);
}