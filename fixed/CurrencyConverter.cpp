#include "CurrencyConverter.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

bool CurrencyConverter::fetchRatesFromAPI(const std::string &base) {
    std::string targetBase = base.empty() ? baseCurrency : base;

    std::string url = apiUrl + targetBase;

    cpr::Response response;
    
    if (!apiKey.empty()) {

        response = cpr::Get(cpr::Url{url},
                           cpr::Header{{"apikey", apiKey}});
    } else {
        response = cpr::Get(cpr::Url{url});
    }

    if (response.status_code != 200) {
        std::cerr << "Failed to fetch exchange rates. HTTP Status: " 
                  << response.status_code << std::endl;
        if (!response.error.message.empty()) {
            std::cerr << "Error: " << response.error.message << std::endl;
        }
        return false;
    }
    
    try {
        json data = json::parse(response.text);

        json rates;
        
        if (data.contains("rates")) {
            rates = data["rates"];
        } else if (data.contains("conversion_rates")) {
            rates = data["conversion_rates"];
        } else if (data.contains("data") && data["data"].contains("rates")) {
            rates = data["data"]["rates"];
        } else {
            rates = data;
        }

        for (auto& [currency, rate] : rates.items()) {
            if (rate.is_number()) {
                updateRate(currency, rate.get<double>());
            }
        }

        if (data.contains("base") && data["base"].is_string()) {
            baseCurrency = data["base"].get<std::string>();
        }

        lastUpdate = std::chrono::system_clock::now();
        
        return true;
        
    } catch (const json::exception& e) {
        std::cerr << "Failed to parse exchange rate JSON: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error processing exchange rates: " << e.what() << std::endl;
        return false;
    }
}

bool CurrencyConverter::fetchRatesForCurrencies(const std::vector<std::string> &currencies, const std::string &base) {

    if (!fetchRatesFromAPI(base)) {
        return false;
    }

    bool allFound = true;
    for (const auto& currency : currencies) {
        if (currency != baseCurrency && exchangeRate.find(currency) == exchangeRate.end()) {
            std::cerr << "Warning: Currency " << currency << " not found in API response" << std::endl;
            allFound = false;
        }
    }
    
    return allFound;
}

bool CurrencyConverter::updateRatesIfNeeded() {
    if (isCacheValid() && !exchangeRate.empty()) {
        return true;
    }

    return fetchRatesFromAPI();
}
