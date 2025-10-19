#include "gtest/gtest.h"
#include "../MovingAverageStrategy.h"
#include "../Transaction.h"
#include <vector>

TEST(MovingAverageStrategyTest, Forecast) {
    MovingAverageStrategy strategy;
    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 10.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 20.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 30.0, 0, "", "cat1"});
    EXPECT_DOUBLE_EQ(strategy.forecast(txs, 2), 25.0);
}

TEST(MovingAverageStrategyTest, ForecastEmpty) {
    MovingAverageStrategy strategy;
    std::vector<Transaction> txs;
    EXPECT_DOUBLE_EQ(strategy.forecast(txs, 2), 0.0);
}

TEST(MovingAverageStrategyTest, ForecastMonthsGreaterThanSize) {
    MovingAverageStrategy strategy;
    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 10.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 20.0, 0, "", "cat1"});
    EXPECT_DOUBLE_EQ(strategy.forecast(txs, 3), 15.0);
}

