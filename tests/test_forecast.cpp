#include "gtest/gtest.h"
#include "../Forecast.h"
#include "../ForecastModel.h"
#include "../Transaction.h"
#include <vector>

TEST(ForecastTest, PredictNext) {
    Forecast forecast;
    forecast.addData(10.0);
    forecast.addData(20.0);
    forecast.addData(30.0);
    EXPECT_DOUBLE_EQ(forecast.predictNext(), 20.0);
}

TEST(ForecastTest, PredictNextEmpty) {
    Forecast forecast;
    EXPECT_DOUBLE_EQ(forecast.predictNext(), 0.0);
}

TEST(ForecastModelTest, ForecastNextMonth) {
    ForecastModel model;
    std::vector<Transaction> txs;
    txs.push_back(Transaction{"", 10.0, 0, "", "cat1"});
    txs.push_back(Transaction{"", 20.0, 0, "", "cat1"});
    EXPECT_DOUBLE_EQ(model.forecastNextMonth(txs), 450.0);
}

TEST(ForecastModelTest, ForecastNextMonthEmpty) {
    ForecastModel model;
    std::vector<Transaction> txs;
    EXPECT_DOUBLE_EQ(model.forecastNextMonth(txs), 0.0);
}

TEST(ForecastModelTest, RollingAverage) {
    ForecastModel model;
    std::vector<double> values = {10.0, 20.0, 30.0, 40.0, 50.0};
    EXPECT_DOUBLE_EQ(model.rollingAverage(values, 3), 40.0);
}

TEST(ForecastModelTest, RollingAverageEmpty) {
    ForecastModel model;
    std::vector<double> values;
    EXPECT_DOUBLE_EQ(model.rollingAverage(values, 3), 0.0);
}

TEST(ForecastModelTest, RollingAverageWindowLargerThanSize) {
    ForecastModel model;
    std::vector<double> values = {10.0, 20.0, 30.0};
    EXPECT_DOUBLE_EQ(model.rollingAverage(values, 5), 20.0);
}

TEST(ForecastModelTest, TrainModel) {
    ForecastModel model;
    std::vector<Transaction> txs;
    // This is a placeholder, so we just check that it doesn't crash.
    model.trainModel(txs);
}

TEST(ForecastModelTest, EvaluateForecast) {
    ForecastModel model;
    std::vector<Transaction> txs;
    // This is a placeholder, so we just check that it doesn't crash.
    EXPECT_DOUBLE_EQ(model.evaluateForecast(txs), 0.0);
}