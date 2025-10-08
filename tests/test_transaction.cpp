#include "gtest/gtest.h"
#include "../Transaction.h"

TEST(TransactionTest, AddTransaction) {
    Transaction t;
    t.addTransaction();
    ASSERT_NE(t.createdAt, 0);
}

TEST(TransactionTest, SplitTransaction) {
    Transaction t;
    t.amount = 100.0;
    t.categoryId = "original";

    std::vector<std::pair<double, std::string>> parts = {{60.0, "cat1"}, {40.0, "cat2"}};
    std::vector<Transaction> split = t.splitTransaction(parts);

    ASSERT_EQ(split.size(), 2);
    EXPECT_EQ(split[0].amount, 60.0);
    EXPECT_EQ(split[0].categoryId, "cat1");
    EXPECT_EQ(split[1].amount, 40.0);
    EXPECT_EQ(split[1].categoryId, "cat2");
}

TEST(TransactionTest, TagTransaction) {
    Transaction t;
    t.tagTransaction("food");
    ASSERT_EQ(t.tags.size(), 1);
    EXPECT_EQ(t.tags[0], "food");

    t.tagTransaction("groceries");
    ASSERT_EQ(t.tags.size(), 2);
    EXPECT_EQ(t.tags[1], "groceries");
}