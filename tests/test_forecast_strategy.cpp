#include "gtest/gtest.h"
#include "../MovingAverageStrategy.h"
#include "../Transaction.h"
#include <vector>

/**
 * @brief Test fixture for MovingAverageStrategy tests.
 * Common setup for all tests: strategy instance and helper for transactions.
 */
class MovingAverageStrategyTest : public ::testing::Test {
protected:
    MovingAverageStrategy strategy;

    static std::vector<Transaction> makeTransactions(const std::vector<double>& amounts,
                                                     const std::string& category = "cat1") {
        std::vector<Transaction> txs;
        long long timestamp = 1000;
        for (double amt : amounts) {
            txs.emplace_back(Transaction{"", amt, timestamp, "", category});
            timestamp += 1000; // simulate chronological order
        }
        return txs;
    }
};

// ---------------- TESTS ----------------

// GIVEN transactions with increasing amounts
// WHEN forecasting using last 2
// THEN the result should be their average
TEST_F(MovingAverageStrategyTest, GivenTransactions_WhenForecastLastTwo_ThenReturnsAverage) {
    auto txs = makeTransactions({10.0, 20.0, 30.0});
    EXPECT_NEAR(strategy.forecast(txs, 2), 25.0, 1e-9);
}

// GIVEN no transactions
// WHEN forecast is called
// THEN result should be 0
TEST_F(MovingAverageStrategyTest, GivenEmptyTransactions_WhenForecastCalled_ThenReturnsZero) {
    std::vector<Transaction> txs;
    EXPECT_NEAR(strategy.forecast(txs, 2), 0.0, 1e-9);
}

// GIVEN fewer transactions than window size
// WHEN forecasting
// THEN average should be over all transactions
TEST_F(MovingAverageStrategyTest, GivenSmallData_WhenWindowLargerThanSize_ThenUsesAllTransactions) {
    auto txs = makeTransactions({10.0, 20.0});
    EXPECT_NEAR(strategy.forecast(txs, 3), 15.0, 1e-9);
}

// GIVEN single transaction
// WHEN forecasted with window=1
// THEN returns that transaction’s amount
TEST_F(MovingAverageStrategyTest, GivenSingleTransaction_WhenWindowOne_ThenReturnsThatValue) {
    auto txs = makeTransactions({42.0});
    EXPECT_NEAR(strategy.forecast(txs, 1), 42.0, 1e-9);
}

// GIVEN multiple transactions
// WHEN forecasted with window=0
// THEN returns 0 (invalid window)
TEST_F(MovingAverageStrategyTest, GivenTransactions_WhenWindowZero_ThenReturnsZero) {
    auto txs = makeTransactions({10.0, 20.0});
    EXPECT_NEAR(strategy.forecast(txs, 0), 0.0, 1e-9);
}

// GIVEN multiple transactions
// WHEN forecasted with negative window
// THEN returns 0 (invalid window)
TEST_F(MovingAverageStrategyTest, GivenTransactions_WhenWindowNegative_ThenReturnsZero) {
    auto txs = makeTransactions({10.0, 20.0});
    EXPECT_NEAR(strategy.forecast(txs, -3), 0.0, 1e-9);
}

// GIVEN transactions of different categories
// WHEN forecasted for cat1
// THEN only cat1 transactions are considered
TEST_F(MovingAverageStrategyTest, GivenMixedCategories_WhenForecastCat1_ThenIgnoresOthers) {
    auto cat1 = makeTransactions({10.0, 30.0}, "cat1");
    auto cat2 = makeTransactions({999.0}, "cat2");
    std::vector<Transaction> txs;
    txs.insert(txs.end(), cat1.begin(), cat1.end());
    txs.insert(txs.end(), cat2.begin(), cat2.end());

    EXPECT_NEAR(strategy.forecast(txs, 3), 20.0, 1e-9);
}
