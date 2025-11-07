#include <benchmark/benchmark.h>
#include "../fixed/Database.h"
#include "../fixed/Transaction.h"
#include <vector>
#include <random>
#include <sstream>

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
        tx.description = "Transaction " + std::to_string(i);
        tx.categoryId = "cat" + std::to_string(i % 5);
        tx.accountId = "acc" + std::to_string(i % 3);
        txs.push_back(tx);
    }
    return txs;
}

static void BM_Database_PersistTransaction(benchmark::State& state) {
    Database db;
    Transaction tx;
    tx.id = "test_tx";
    tx.amount = 100.0;
    tx.date = 1672531200;
    tx.description = "Test transaction";
    tx.categoryId = "cat1";
    tx.accountId = "acc1";
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(db.persistTransaction(tx));
    }
}
BENCHMARK(BM_Database_PersistTransaction);

static void BM_Database_PersistTransaction_Bulk(benchmark::State& state) {
    Database db;
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (auto _ : state) {
        for (const auto& tx : txs) {
            db.persistTransaction(tx);
        }
        db.txStore.clear();
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_Database_PersistTransaction_Bulk)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_Database_QueryTransactions(benchmark::State& state) {
    Database db;
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));

    for (const auto& tx : txs) {
        db.persistTransaction(tx);
    }
    
    for (auto _ : state) {
        auto result = db.queryTransactions();
        benchmark::DoNotOptimize(result);
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_Database_QueryTransactions)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_Database_InsertThenQuery(benchmark::State& state) {
    Database db;
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)));
    
    for (auto _ : state) {

        for (const auto& tx : txs) {
            db.persistTransaction(tx);
        }
        auto result = db.queryTransactions();
        benchmark::DoNotOptimize(result);
        db.txStore.clear();
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_Database_InsertThenQuery)
    ->RangeMultiplier(2)->Range(100, 10000)
    ->Complexity(benchmark::oN);


