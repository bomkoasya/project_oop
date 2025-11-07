#include <benchmark/benchmark.h>
#include "../fixed/MovingAverageStrategy.h"
#include "../fixed/ExponentialSmoothingStrategy.h"
#include "../fixed/Transaction.h"
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>

static std::vector<Transaction> generateTransactions(size_t count) {
    std::vector<Transaction> txs;
    txs.reserve(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> amount_dist(10.0, 1000.0);
    std::uniform_int_distribution<int> cat_dist(0, 4);
    
    for (size_t i = 0; i < count; ++i) {
        Transaction tx;
        std::ostringstream oss;
        oss << "tx_" << i;
        tx.id = oss.str();
        tx.amount = amount_dist(gen);
        tx.date = 1672531200 + i * 86400;
        tx.description = "Transaction " + std::to_string(i);
        tx.categoryId = "cat" + std::to_string(cat_dist(gen));
        tx.accountId = "acc" + std::to_string(i % 3);
        txs.push_back(tx);
    }
    return txs;
}

static void BM_MovingAverageStrategy_Forecast(benchmark::State& state) {
    MovingAverageStrategy strategy;
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    int months = 3;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(strategy.forecast(txs, months));
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_MovingAverageStrategy_Forecast)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oNLogN);

static void BM_MovingAverageStrategy_Forecast_VaryingMonths(benchmark::State& state) {
    MovingAverageStrategy strategy;
    auto txs = generateTransactions(10000);
    int months = static_cast<int>(state.range(0));
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(strategy.forecast(txs, months));
    }
    
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_MovingAverageStrategy_Forecast_VaryingMonths)
    ->RangeMultiplier(2)->Range(1, 12);

static void BM_ExponentialSmoothingStrategy_Forecast(benchmark::State& state) {
    ExponentialSmoothingStrategy strategy(0.5);
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    int months = 12;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(strategy.forecast(txs, months));
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_ExponentialSmoothingStrategy_Forecast)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_ExponentialSmoothingStrategy_Forecast_VaryingAlpha(benchmark::State& state) {
    double alpha = state.range(0) / 100.0;
    ExponentialSmoothingStrategy strategy(alpha);
    auto txs = generateTransactions(10000);
    int months = 12;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(strategy.forecast(txs, months));
    }
}
BENCHMARK(BM_ExponentialSmoothingStrategy_Forecast_VaryingAlpha)
    ->DenseRange(10, 90, 10);

static void BM_Strategy_Comparison(benchmark::State& state) {
    MovingAverageStrategy ma_strategy;
    ExponentialSmoothingStrategy es_strategy(0.5);
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    int months = 3;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(ma_strategy.forecast(txs, months));
        benchmark::DoNotOptimize(es_strategy.forecast(txs, months));
    }
    
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_Strategy_Comparison)
    ->RangeMultiplier(2)->Range(100, 10000);


