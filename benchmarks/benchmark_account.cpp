#include <benchmark/benchmark.h>
#include "../fixed/Account.h"
#include "../fixed/Transaction.h"
#include <vector>
#include <random>
#include <sstream>

static std::vector<Transaction> generateTransactions(size_t count, const std::string& accountId) {
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
        tx.categoryId = "cat" + std::to_string(i % 5);
        tx.accountId = accountId;
        txs.push_back(tx);
    }
    return txs;
}

static void BM_Account_ReconcileAccount(benchmark::State& state) {
    Account account;
    account.id = "acc1";
    account.balance = 5000.0;
    account.name = "Test Account";
    account.currency = "USD";
    
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)), account.id);

    double sum = 0.0;
    for (const auto& tx : txs) {
        sum += tx.amount;
    }
    account.balance = sum;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(account.reconcileAccount(txs));
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_Account_ReconcileAccount)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_Account_ReconcileAccount_Mismatch(benchmark::State& state) {
    Account account;
    account.id = "acc1";
    account.balance = 9999.99;
    account.name = "Test Account";
    account.currency = "USD";
    
    auto txs = generateTransactions(static_cast<size_t>(state.range(0)), account.id);
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(account.reconcileAccount(txs));
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_Account_ReconcileAccount_Mismatch)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_Account_ReconcileAccount_MultipleAccounts(benchmark::State& state) {
    Account account;
    account.id = "acc1";
    account.balance = 5000.0;
    account.name = "Test Account";
    account.currency = "USD";
    
    std::vector<Transaction> txs;
    txs.reserve(static_cast<size_t>(state.range(0)));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> amount_dist(-1000.0, 1000.0);
    std::uniform_int_distribution<int> acc_dist(0, 4);
    
    for (int i = 0; i < state.range(0); ++i) {
        Transaction tx;
        tx.id = "tx_" + std::to_string(i);
        tx.amount = amount_dist(gen);
        tx.date = 1672531200 + i * 86400;
        tx.description = "Transaction " + std::to_string(i);
        tx.categoryId = "cat" + std::to_string(i % 5);
        tx.accountId = "acc" + std::to_string(acc_dist(gen));
        txs.push_back(tx);
    }

    double sum = 0.0;
    for (const auto& tx : txs) {
        if (tx.accountId == account.id) {
            sum += tx.amount;
        }
    }
    account.balance = sum;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(account.reconcileAccount(txs));
    }
    
    state.SetComplexityN(state.range(0));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
BENCHMARK(BM_Account_ReconcileAccount_MultipleAccounts)
    ->RangeMultiplier(2)->Range(100, 100000)
    ->Complexity(benchmark::oN);

static void BM_Account_Persist(benchmark::State& state) {
    Account account;
    account.id = "acc1";
    account.balance = 5000.0;
    account.name = "Test Account";
    account.currency = "USD";
    
    for (auto _ : state) {
        account.persist();
    }
}
BENCHMARK(BM_Account_Persist);

static void BM_CheckingAccount_Type(benchmark::State& state) {
    CheckingAccount account;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(account.type());
    }
}
BENCHMARK(BM_CheckingAccount_Type);

static void BM_CreditAccount_Type(benchmark::State& state) {
    CreditAccount account;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(account.type());
    }
}
BENCHMARK(BM_CreditAccount_Type);

static void BM_SavingsAccount_Type(benchmark::State& state) {
    SavingsAccount account;
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(account.type());
    }
}
BENCHMARK(BM_SavingsAccount_Type);


