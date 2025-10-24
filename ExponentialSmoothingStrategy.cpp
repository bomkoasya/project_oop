#include "ExponentialSmoothingStrategy.h"
#include <vector>

double ExponentialSmoothingStrategy::forecast(const std::vector<Transaction>& txs, int months) {
    if (txs.empty()) {
        return 0.0;
    }

    double smoothed = txs[0].amount; 

    for (size_t i = 1; i < txs.size(); ++i) {
        smoothed = alpha * txs[i].amount + (1.0 - alpha) * smoothed;
    }

    return smoothed;
}