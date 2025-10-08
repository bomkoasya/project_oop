#include "gtest/gtest.h"
#include "../Account.h"
#include "../Transaction.h"
#include <vector>

TEST(AccountTest, ReconcileAccount_Success) {
    Account acc;
    acc.id = "acc1";
    acc.balance = 150.0;

    std::vector<Transaction> transactions;
    Transaction t1;
    t1.accountId = "acc1";
    t1.amount = 100.0;
    transactions.push_back(t1);

    Transaction t2;
    t2.accountId = "acc1";
    t2.amount = 50.0;
    transactions.push_back(t2);

    EXPECT_TRUE(acc.reconcileAccount(transactions));
}

TEST(AccountTest, ReconcileAccount_Failure) {
    Account acc;
    acc.id = "acc1";
    acc.balance = 140.0; // Incorrect balance

    std::vector<Transaction> transactions;
    Transaction t1;
    t1.accountId = "acc1";
    t1.amount = 100.0;
    transactions.push_back(t1);

    Transaction t2;
    t2.accountId = "acc1";
    t2.amount = 50.0;
    transactions.push_back(t2);

    EXPECT_FALSE(acc.reconcileAccount(transactions));
}

TEST(AccountTest, ReconcileAccount_NoTransactions) {
    Account acc;
    acc.id = "acc1";
    acc.balance = 0.0;

    std::vector<Transaction> transactions;
    EXPECT_TRUE(acc.reconcileAccount(transactions));
}

TEST(AccountTest, AccountSubtypes) {
    CheckingAccount checking;
    EXPECT_EQ(checking.type(), "CheckingAccount");

    CreditAccount credit;
    EXPECT_EQ(credit.type(), "CreditAccount");

    SavingsAccount savings;
    EXPECT_EQ(savings.type(), "SavingsAccount");
}