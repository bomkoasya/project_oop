#pragma once
#include <vector>

class Forecast {
public:
    // Додаємо місячний дохід/витрати
    void addData(double value);

    // Прогноз на наступний місяць
    double predictNext() const;

private:
    std::vector<double> data;
};
