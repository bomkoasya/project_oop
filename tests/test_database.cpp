#include "gtest/gtest.h"
#include "../Database.h"
#include "../Transaction.h"

class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        t1.id = "tx1";
        t1.amount = 100.0;

        t2.id = "tx2";
        t2.amount = 200.0;
    }

    Database db;
    Transaction t1, t2;
};

TEST_F(DatabaseTest, PersistTransaction) {
    ASSERT_TRUE(db.persistTransaction(t1));
    ASSERT_EQ(db.txStore.size(), 1);
    EXPECT_EQ(db.txStore["tx1"].amount, 100.0);

    ASSERT_TRUE(db.persistTransaction(t2));
    ASSERT_EQ(db.txStore.size(), 2);
    EXPECT_EQ(db.txStore["tx2"].amount, 200.0);
}

TEST_F(DatabaseTest, QueryTransactions) {
    db.persistTransaction(t1);
    db.persistTransaction(t2);

    std::vector<Transaction> transactions = db.queryTransactions();
    ASSERT_EQ(transactions.size(), 2);

    bool found_t1 = false;
    bool found_t2 = false;
    for (const auto& t : transactions) {
        if (t.id == "tx1") {
            EXPECT_EQ(t.amount, 100.0);
            found_t1 = true;
        } else if (t.id == "tx2") {
            EXPECT_EQ(t.amount, 200.0);
            found_t2 = true;
        }
    }
    EXPECT_TRUE(found_t1);
    EXPECT_TRUE(found_t2);
}

TEST_F(DatabaseTest, QueryTransactions_Empty) {
    std::vector<Transaction> transactions = db.queryTransactions();
    EXPECT_TRUE(transactions.empty());
}