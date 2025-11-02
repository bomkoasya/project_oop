#pragma once
#include <vector>

class Forecast {
public:

    void addData(double value);

    double predictNext() const;

private:
    std::vector<double> data;
};
