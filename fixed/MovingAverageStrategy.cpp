#include "MovingAverageStrategy.h"
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>

double MovingAverageStrategy::forecast(const std::vector<Transaction>& transactions, int months) {
    if (transactions.empty() || months <= 0) {
        return 0.0;
    }

    const std::string targetCategory = transactions.front().categoryId;

    std::vector<const Transaction*> filtered;
    filtered.reserve(transactions.size());
    for (const auto& t : transactions) {
        if (t.categoryId == targetCategory) {
            filtered.push_back(&t);
        }
    }

    if (filtered.empty()) {
        return 0.0;
    }

    std::sort(filtered.begin(), filtered.end(),
              [](const Transaction* a, const Transaction* b) {
                  return a->date < b->date;
              });

    const std::size_t total = filtered.size();
    const std::size_t window = static_cast<std::size_t>(months);
    const std::size_t start_idx = (window >= total) ? 0 : (total - window);

    double sum = 0.0;
    std::size_t count = 0;
    for (std::size_t i = start_idx; i < total; ++i) {
        sum += filtered[i]->amount;
        ++count;
    }

    if (count == 0) {
        return 0.0;
    }

    return sum / static_cast<double>(count);
}
