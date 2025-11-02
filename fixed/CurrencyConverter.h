#pragma once
#include <string>
#include <map>


class CurrencyConverter {
public:
    std::string baseCurrency = "USD";
    std::map<std::string, double> exchangeRate;


    // Methods
    double convert(double amount, const std::string &from, const std::string &to);
    void updateRate(const std::string &currency, double rate);
};


inline double CurrencyConverter::convert(double amount, const std::string &from, const std::string &to) {
    if (from == to) return amount;
    double fromRate = (exchangeRate.count(from) ? exchangeRate[from] : 1.0);
    double toRate = (exchangeRate.count(to) ? exchangeRate[to] : 1.0);
    if (from == baseCurrency) return amount * toRate;
    if (to == baseCurrency) return amount / fromRate;
    // convert via base
    double inBase = amount / fromRate;
    return inBase * toRate;
}
inline void CurrencyConverter::updateRate(const std::string &currency, double rate) { exchangeRate[currency] = rate; }