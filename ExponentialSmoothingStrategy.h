#pragma once
#include "ForecastStrategy.h"

class ExponentialSmoothingStrategy : public ForecastStrategy {
public:
    double alpha;
    ExponentialSmoothingStrategy(double a = 0.5) : alpha(a) {}
    double forecast(const std::vector<Transaction>& history, int months) override;
};
