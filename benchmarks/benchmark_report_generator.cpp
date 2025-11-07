#include <benchmark/benchmark.h>
#include "../fixed/ReportGenerator.h"
#include "../fixed/Transaction.h"
#include <vector>
#include <random>
#include <sstream>

static std::vector<Transaction> generateTransactions(size_t count) {
    std::vector<Transaction> txs;
    txs.reserve(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> amount_dist(-1000.0, 1000.0);
    
    for (size_t i = 0; i < count; ++i) {
        Transaction tx;
        std::ostringstream oss;
        oss << "tx_" << i;
        tx.id = oss.str();
        tx.amount = amount_dist(gen);
        tx.date = 1672531200 + i * 86400;
        tx.description = "Transaction " + std::to_string(i);
        tx.categoryId = "cat" + std::to_string(i % 10);
        tx.accountId = "acc" + std::to_string(i % 5);
        txs.push_back(tx);
    }
    return txs;
}

static void BM_ReportGenerator_CalculateTotal(benchmark::State& state) {
    ReportGenerator generator("Test Report");
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (const auto& tx : txs) {
        generator.addTransaction(tx);
    }
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(generator.calculateTotal());
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_ReportGenerator_CalculateTotal)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_ReportGenerator_CalculateByCategory(benchmark::State& state) {
    ReportGenerator generator("Test Report");
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (const auto& tx : txs) {
        generator.addTransaction(tx);
    }
    
    std::string targetCategory = "cat5";
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(generator.calculateByCategory(targetCategory));
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_ReportGenerator_CalculateByCategory)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_ReportGenerator_CalculateByAccount(benchmark::State& state) {
    ReportGenerator generator("Test Report");
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (const auto& tx : txs) {
        generator.addTransaction(tx);
    }
    
    std::string targetAccount = "acc2";
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(generator.calculateByAccount(targetAccount));
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_ReportGenerator_CalculateByAccount)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_ReportGenerator_PreparePieChartData(benchmark::State& state) {
    ReportGenerator generator("Test Report");
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (const auto& tx : txs) {
        generator.addTransaction(tx);
    }
    
    for (auto _ : state) {
        auto result = generator.preparePieChartData();
        benchmark::DoNotOptimize(result);
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_ReportGenerator_PreparePieChartData)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_ReportGenerator_AddTransaction(benchmark::State& state) {
    ReportGenerator generator("Test Report");
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (auto _ : state) {
        for (const auto& tx : txs) {
            generator.addTransaction(tx);
        }
        generator.transactions.clear();
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_ReportGenerator_AddTransaction)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);


