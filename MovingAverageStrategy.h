#pragma once
#include "ForecastStrategy.h"
#include <vector>

class MovingAverageStrategy : public ForecastStrategy {
public:
    double forecast(const std::vector<Transaction>& history, int months) override;
};
