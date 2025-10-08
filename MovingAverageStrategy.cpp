#include "MovingAverageStrategy.h"

double MovingAverageStrategy::forecast(const std::vector<Transaction>& history, int months) {
    if (history.empty()) return 0.0;

    double sum = 0.0;
    int count = 0;

    for (int i = static_cast<int>(history.size()) - 1; i >= 0 && count < months; --i) {
        sum += history[i].amount;
        count++;
    }

    return (count > 0) ? (sum / count) : 0.0;
}
