#pragma once
#include <vector>
#include "Transaction.h"

class ForecastStrategy {
public:
    virtual ~ForecastStrategy() {}
    virtual double forecast(const std::vector<Transaction>& history, int months) = 0;
};
