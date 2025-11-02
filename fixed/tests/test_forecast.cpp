#include "gtest/gtest.h"
#include "../Forecast.h"
#include "../ForecastModel.h"
#include "../Transaction.h"
#include <filesystem>
#include <vector>

class ForecastTestFixture : public ::testing::Test {
protected:
    Forecast forecast;
};

class ForecastModelTestFixture : public ::testing::Test {
protected:
    ForecastModel model;
};

TEST_F(ForecastTestFixture, GIVEN_Data_WHEN_PredictNextCalled_THEN_ReturnsAverageDifference) {
    forecast.addData(10.0);
    forecast.addData(20.0);
    forecast.addData(30.0);
    EXPECT_NEAR(forecast.predictNext(), 20.0, 1e-9);
}

TEST_F(ForecastTestFixture, GIVEN_EmptyData_WHEN_PredictNextCalled_THEN_ReturnsZero) {
    EXPECT_DOUBLE_EQ(forecast.predictNext(), 0.0);
}

TEST_F(ForecastModelTestFixture, GIVEN_Transactions_WHEN_ForecastNextMonthCalled_THEN_ReturnsExpectedValue) {
    std::vector<Transaction> txs = {
        {"", 10.0, 0, "", "cat1"},
        {"", 20.0, 0, "", "cat1"}
    };
    double expected = (10.0 + 20.0) / 2 * 30;
    EXPECT_NEAR(model.forecastNextMonth(txs), expected, 1e-9);
}

TEST_F(ForecastModelTestFixture, GIVEN_EmptyTransactions_WHEN_ForecastNextMonthCalled_THEN_ReturnsZero) {
    std::vector<Transaction> txs;
    EXPECT_DOUBLE_EQ(model.forecastNextMonth(txs), 0.0);
}

TEST_F(ForecastModelTestFixture, GIVEN_Values_WHEN_RollingAverageCalled_THEN_ReturnsCorrectAverage) {
    std::vector<double> values = {10.0, 20.0, 30.0, 40.0, 50.0};
    EXPECT_NEAR(model.rollingAverage(values, 3), 40.0, 1e-9);
}

TEST_F(ForecastModelTestFixture, GIVEN_EmptyValues_WHEN_RollingAverageCalled_THEN_ReturnsZero) {
    std::vector<double> values;
    EXPECT_DOUBLE_EQ(model.rollingAverage(values, 3), 0.0);
}

TEST_F(ForecastModelTestFixture, GIVEN_WindowLargerThanSize_WHEN_RollingAverageCalled_THEN_UsesAllValues) {
    std::vector<double> values = {10.0, 20.0, 30.0};
    EXPECT_NEAR(model.rollingAverage(values, 5), 20.0, 1e-9);
}

TEST_F(ForecastModelTestFixture, GIVEN_Transactions_WHEN_TrainModelCalled_THEN_DoesNotThrow) {
    std::vector<Transaction> txs;
    EXPECT_NO_THROW(model.trainModel(txs));
}

TEST_F(ForecastModelTestFixture, GIVEN_Transactions_WHEN_EvaluateForecastCalled_THEN_ReturnsZero) {
    std::vector<Transaction> txs;
    EXPECT_DOUBLE_EQ(model.evaluateForecast(txs), 0.0);
}
