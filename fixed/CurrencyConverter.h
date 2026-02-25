#pragma once
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <functional>

/**
 * @class CurrencyConverter
 * @brief Handles currency conversion with automatic exchange rate updates from API
 * 
 * Supports fetching exchange rates from external APIs and converting between currencies.
 * Uses cpr (C++ Requests) library for HTTP requests to banking/financial APIs.
 */
class CurrencyConverter {
public:
    std::string baseCurrency = "USD";
    std::map<std::string, double> exchangeRate;

    std::string apiKey;
    std::string apiUrl = "https://api.exchangerate-api.com/v4/latest/";  // Default free API

    std::chrono::system_clock::time_point lastUpdate;
    std::chrono::minutes cacheTimeout{60};

    double convert(double amount, const std::string &from, const std::string &to);
    void updateRate(const std::string &currency, double rate);
    
    /**
     * @brief Fetches exchange rates from the configured API
     * @param base Base currency code (default: uses baseCurrency member)
     * @return true if rates were successfully fetched and updated
     */
    bool fetchRatesFromAPI(const std::string &base = "");
    
    /**
     * @brief Fetches exchange rates for specific currencies
     * @param currencies Vector of currency codes to fetch
     * @param base Base currency code (default: uses baseCurrency member)
     * @return true if rates were successfully fetched
     */
    bool fetchRatesForCurrencies(const std::vector<std::string> &currencies, const std::string &base = "");
    
    void fetchRatesFromAPIAsync(std::function<void(bool)> callback, const std::string &base = "");

    /**
     * @brief Automatically updates rates if cache has expired
     * @return true if rates were updated (or already fresh)
     */
    bool updateRatesIfNeeded();
    
    /**
     * @brief Sets a custom API URL (for different exchange rate providers)
     * @param url API endpoint URL
     */
    void setApiUrl(const std::string &url);
    
    /**
     * @brief Sets API key for services that require authentication
     * @param key API key string
     */
    void setApiKey(const std::string &key);
    
    /**
     * @brief Checks if cached rates are still valid
     * @return true if rates are fresh (within cache timeout)
     */
    bool isCacheValid() const;
};

inline double CurrencyConverter::convert(double amount, const std::string &from, const std::string &to) {
    if (from == to) return amount;
    double fromRate = (exchangeRate.count(from) ? exchangeRate[from] : 1.0);
    double toRate = (exchangeRate.count(to) ? exchangeRate[to] : 1.0);
    if (from == baseCurrency) return amount * toRate;
    if (to == baseCurrency) return amount / fromRate;
    double inBase = amount / fromRate;
    return inBase * toRate;
}

inline void CurrencyConverter::updateRate(const std::string &currency, double rate) { 
    exchangeRate[currency] = rate; 
}

inline void CurrencyConverter::setApiUrl(const std::string &url) {
    apiUrl = url;
}

inline void CurrencyConverter::setApiKey(const std::string &key) {
    apiKey = key;
}

inline bool CurrencyConverter::isCacheValid() const {
    auto now = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::minutes>(now - lastUpdate);
    return elapsed < cacheTimeout;
}
