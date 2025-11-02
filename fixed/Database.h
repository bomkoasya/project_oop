#pragma once
#include <string>
#include <vector>
#include <map>
#include "Transaction.h"

class Database {
public:
    std::string dbName;
    std::string dbPath;
    std::map<std::string, Transaction> txStore;

    bool persistTransaction(const Transaction &tx);
    bool persistAccount(const std::string & /*accountId*/);
    bool backup(const std::string &targetPath);
    bool restore(const std::string &sourcePath);
    bool encryptData(const std::string & /*passphrase*/);

    std::vector<Transaction> queryTransactions() const;
};

inline bool Database::persistTransaction(const Transaction &tx) {
    txStore[tx.id] = tx;
    return true;
}
inline bool Database::persistAccount(const std::string & /*accountId*/) { return true; }
inline bool Database::backup(const std::string & /*targetPath*/) { return true; }
inline bool Database::restore(const std::string & /*sourcePath*/) { return true; }
inline bool Database::encryptData(const std::string & /*passphrase*/) { return true; }

inline std::vector<Transaction> Database::queryTransactions() const {
    std::vector<Transaction> out;
    for (const auto &kv : txStore) out.push_back(kv.second);
    return out;
}