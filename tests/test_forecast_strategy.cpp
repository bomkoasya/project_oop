#include "gtest/gtest.h"
#include "../MovingAverageStrategy.h"
#include "../ExponentialSmoothingStrategy.h"
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

TEST(ExponentialSmoothingStrategyTest, Forecast) {
    ExponentialSmoothingStrategy strategy(0.5);
    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 10.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 20.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 30.0, 0, "", "cat1"});
    EXPECT_DOUBLE_EQ(strategy.forecast(txs, 1), 22.5);
}

TEST(ExponentialSmoothingStrategyTest, ForecastEmpty) {
    ExponentialSmoothingStrategy strategy;
    std::vector<Transaction> txs;
    EXPECT_DOUBLE_EQ(strategy.forecast(txs, 1), 0.0);
}

TEST(ExponentialSmoothingStrategyTest, ForecastDifferentAlpha) {
    ExponentialSmoothingStrategy strategy(0.8);
    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 10.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 20.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 30.0, 0, "", "cat1"});
    EXPECT_NEAR(strategy.forecast(txs, 1), 27.6, 1e-9);
}