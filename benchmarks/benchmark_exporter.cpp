#include <benchmark/benchmark.h>
#include "../fixed/Exporter.h"
#include "../fixed/Transaction.h"
#include <vector>
#include <random>
#include <sstream>
#include <filesystem>
#include <cstdio>

static std::vector<Transaction> generateTransactions(size_t count) {
    std::vector<Transaction> txs;
    txs.reserve(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> amount_dist(10.0, 1000.0);
    
    for (size_t i = 0; i < count; ++i) {
        Transaction tx;
        std::ostringstream oss;
        oss << "tx_" << i;
        tx.id = oss.str();
        tx.amount = amount_dist(gen);
        tx.date = 1672531200 + i * 86400;
        tx.description = "Transaction " + std::to_string(i) + " description with some text";
        tx.categoryId = "cat" + std::to_string(i % 5);
        tx.accountId = "acc" + std::to_string(i % 3);
        txs.push_back(tx);
    }
    return txs;
}

static void cleanupFile(const std::string& filename) {
    if (std::filesystem::exists(filename)) {
        std::remove(filename.c_str());
    }
}

static void BM_CSVExporter_ExportData(benchmark::State& state) {
    std::string filename = "benchmark_test_export.csv";
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (auto _ : state) {
        CSVExporter exporter(filename);
        benchmark::DoNotOptimize(exporter.exportData(txs));
        cleanupFile(filename);
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_CSVExporter_ExportData)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_ExcelExporter_ExportData(benchmark::State& state) {
    std::string filename = "benchmark_test_export.xlsx";
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (auto _ : state) {
        ExcelExporter exporter(filename);
        benchmark::DoNotOptimize(exporter.exportData(txs));
        cleanupFile(filename);
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_ExcelExporter_ExportData)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_CSVExporter_Construction(benchmark::State& state) {
    std::string filename = "benchmark_test_construction.csv";
    
    for (auto _ : state) {
        CSVExporter exporter(filename);
        benchmark::DoNotOptimize(&exporter);
        cleanupFile(filename);
    }
}
BENCHMARK(BM_CSVExporter_Construction);

static void BM_CSVExporter_ExportData_SpecialChars(benchmark::State& state) {
    std::string filename = "benchmark_test_special.csv";
    std::vector<Transaction> txs;
    txs.reserve(static_cast<size_t>(state.range(0)));
    
    for (int i = 0; i < state.range(0); ++i) {
        Transaction tx;
        tx.id = "tx_" + std::to_string(i);
        tx.amount = 100.0 + i;
        tx.date = 1672531200 + i * 86400;
        tx.description = "Transaction with \"quotes\", commas, and\nnewlines";
        tx.categoryId = "cat" + std::to_string(i % 5);
        tx.accountId = "acc" + std::to_string(i % 3);
        txs.push_back(tx);
    }
    
    for (auto _ : state) {
        CSVExporter exporter(filename);
        benchmark::DoNotOptimize(exporter.exportData(txs));
        cleanupFile(filename);
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_CSVExporter_ExportData_SpecialChars)
    ->RangeMultiplier(2)->Range(100, 10000)
    ->Complexity(benchmark::oN);

