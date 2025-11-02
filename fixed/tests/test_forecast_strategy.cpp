#include "gtest/gtest.h"
#include "../MovingAverageStrategy.h"
#include "../Transaction.h"
#include <vector>

class MovingAverageStrategyTest : public ::testing::Test {
protected:
    MovingAverageStrategy strategy;

    static std::vector<Transaction> makeTransactions(const std::vector<double>& amounts,
                                                     const std::string& category = "cat1") {
        std::vector<Transaction> txs;
        long long timestamp = 1000;
        for (double amt : amounts) {
            txs.emplace_back(Transaction{"", amt, timestamp, "", category});
            timestamp += 1000;
        }
        return txs;
    }
};

TEST_F(MovingAverageStrategyTest, GivenTransactions_WhenForecastLastTwo_ThenReturnsAverage) {
    auto txs = makeTransactions({10.0, 20.0, 30.0});
    EXPECT_NEAR(strategy.forecast(txs, 2), 25.0, 1e-9);
}

TEST_F(MovingAverageStrategyTest, GivenEmptyTransactions_WhenForecastCalled_ThenReturnsZero) {
    std::vector<Transaction> txs;
    EXPECT_NEAR(strategy.forecast(txs, 2), 0.0, 1e-9);
}

TEST_F(MovingAverageStrategyTest, GivenSmallData_WhenWindowLargerThanSize_ThenUsesAllTransactions) {
    auto txs = makeTransactions({10.0, 20.0});
    EXPECT_NEAR(strategy.forecast(txs, 3), 15.0, 1e-9);
}

TEST_F(MovingAverageStrategyTest, GivenSingleTransaction_WhenWindowOne_ThenReturnsThatValue) {
    auto txs = makeTransactions({42.0});
    EXPECT_NEAR(strategy.forecast(txs, 1), 42.0, 1e-9);
}

TEST_F(MovingAverageStrategyTest, GivenTransactions_WhenWindowZero_ThenReturnsZero) {
    auto txs = makeTransactions({10.0, 20.0});
    EXPECT_NEAR(strategy.forecast(txs, 0), 0.0, 1e-9);
}

TEST_F(MovingAverageStrategyTest, GivenTransactions_WhenWindowNegative_ThenReturnsZero) {
    auto txs = makeTransactions({10.0, 20.0});
    EXPECT_NEAR(strategy.forecast(txs, -3), 0.0, 1e-9);
}

TEST_F(MovingAverageStrategyTest, GivenMixedCategories_WhenForecastCat1_ThenIgnoresOthers) {
    auto cat1 = makeTransactions({10.0, 30.0}, "cat1");
    auto cat2 = makeTransactions({999.0}, "cat2");
    std::vector<Transaction> txs;
    txs.insert(txs.end(), cat1.begin(), cat1.end());
    txs.insert(txs.end(), cat2.begin(), cat2.end());

    EXPECT_NEAR(strategy.forecast(txs, 3), 20.0, 1e-9);
}
