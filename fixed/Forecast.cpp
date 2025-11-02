#include "Forecast.h"

void Forecast::addData(double value) {
    data.push_back(value);
}

double Forecast::predictNext() const {
    if (data.empty()) return 0.0;

    double sum = 0.0;
    for (double val : data)
        sum += val;

    return sum / data.size();
}
