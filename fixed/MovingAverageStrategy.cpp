#include "MovingAverageStrategy.h"
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>

double MovingAverageStrategy::forecast(const std::vector<Transaction>& transactions, int months) {
    if (transactions.empty() || months <= 0) {
        return 0.0;
    }


    std::vector<const Transaction*> sorted_transactions;
    sorted_transactions.reserve(transactions.size());

    for (const auto& t : transactions) {
        sorted_transactions.push_back(&t);
    }

    std::sort(sorted_transactions.begin(), sorted_transactions.end(),
              [](const Transaction* a, const Transaction* b) {
                  return a->date < b->date;
              });

    const std::size_t total = sorted_transactions.size();
    const std::size_t window = static_cast<std::size_t>(months);
    const std::size_t start_idx = (window >= total) ? 0 : (total - window);

    double sum = 0.0;
    std::size_t count = 0;
    for (std::size_t i = start_idx; i < total; ++i) {
        sum += sorted_transactions[i]->amount;
        ++count;
    }

    if (count == 0) {
        return 0.0;
    }

    return sum / static_cast<double>(count);
}
