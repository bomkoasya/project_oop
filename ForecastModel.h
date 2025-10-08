#pragma once
#include <vector>
#include <cmath>
#include "Transaction.h"


class ForecastModel {
public:
    int forecastHorizon = 30; // days by default
    double confidence = 0.8;


    double forecastNextMonth(const std::vector<Transaction>& history);
    double rollingAverage(const std::vector<double>& values, int window);
    void trainModel(const std::vector<Transaction>& history);
    double evaluateForecast(const std::vector<Transaction>& actual);
};


inline double ForecastModel::forecastNextMonth(const std::vector<Transaction>& history) {

    if (history.empty()) return 0.0;
    double sum = 0.0;
    for (const auto &t : history) sum += t.amount;
    double avg = sum / static_cast<double>(history.size());
    return avg * 30.0;
}
inline double ForecastModel::rollingAverage(const std::vector<double>& values, int window) {
    if (values.empty() || window <= 0) return 0.0;
    double sum = 0.0;
    int count = 0;
    for (int i = std::max(0, (int)values.size() - window); i < (int)values.size(); ++i) { sum += values[i]; ++count; }
    return count ? sum / count : 0.0;
}
inline void ForecastModel::trainModel(const std::vector<Transaction>& /*history*/) { /* placeholder */ }
inline double ForecastModel::evaluateForecast(const std::vector<Transaction>& /*actual*/) { return 0.0; }