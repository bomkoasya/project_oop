#include <benchmark/benchmark.h>
#include "../fixed/Forecast.h"
#include "../fixed/ForecastModel.h"
#include "../fixed/Transaction.h"
#include <vector>
#include <random>
#include <numeric>

static std::vector<Transaction> generateTransactions(size_t count) {
    std::vector<Transaction> txs;
    txs.reserve(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> amount_dist(10.0, 1000.0);
    
    for (size_t i = 0; i < count; ++i) {
        Transaction tx;
        tx.id = "tx_" + std::to_string(i);
        tx.amount = amount_dist(gen);
        tx.date = 1672531200 + i * 86400;
        tx.description = "Transaction " + std::to_string(i);
        tx.categoryId = "cat" + std::to_string(i % 5);
        tx.accountId = "acc" + std::to_string(i % 3);
        txs.push_back(tx);
    }
    return txs;
}

static void BM_Forecast_PredictNext(benchmark::State& state) {
    Forecast forecast;
    std::vector<double> data(static_cast<size_t>(state.range(0)));
    std::iota(data.begin(), data.end(), 10.0);
    
    for (double val : data) {
        forecast.addData(val);
    }
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(forecast.predictNext());
    }
    
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_Forecast_PredictNext)
    ->RangeMultiplier(2)->Range(10, 10000)
    ->Complexity(benchmark::oN);

static void BM_ForecastModel_ForecastNextMonth(benchmark::State& state) {
    ForecastModel model;
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(model.forecastNextMonth(txs));
    }
    
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_ForecastModel_ForecastNextMonth)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_ForecastModel_RollingAverage(benchmark::State& state) {
    ForecastModel model;
    std::vector<double> values(static_cast<size_t>(state.range(0)));
    std::iota(values.begin(), values.end(), 1.0);
    int window = static_cast<int>(state.range(0) / 4);
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(model.rollingAverage(values, window));
    }
    
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_ForecastModel_RollingAverage)
    ->RangeMultiplier(2)->Range(100, 10000)
    ->Complexity(benchmark::oN);

static void BM_ForecastModel_TrainModel(benchmark::State& state) {
    ForecastModel model;
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (auto _ : state) {
        model.trainModel(txs);
    }
    
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_ForecastModel_TrainModel)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);


