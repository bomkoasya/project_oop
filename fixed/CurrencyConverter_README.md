# CurrencyConverter with HTTP API Support

The `CurrencyConverter` class now supports automatic exchange rate updates from external APIs using the cpr (C++ Requests) library.

## Features

- **Automatic Rate Updates**: Fetch exchange rates from banking/financial APIs
- **Caching**: Rates are cached to avoid excessive API calls
- **Multiple API Support**: Configurable API endpoints
- **Backward Compatible**: Existing manual rate update methods still work

## Usage Examples

### Basic Usage with API

```cpp
#include "CurrencyConverter.h"
#include <iostream>

int main() {
    CurrencyConverter converter;
    
    // Fetch rates from free API (no key required)
    if (converter.fetchRatesFromAPI("USD")) {
        std::cout << "Rates updated successfully!" << std::endl;
        
        // Convert currencies
        double amount = converter.convert(100.0, "USD", "EUR");
        std::cout << "100 USD = " << amount << " EUR" << std::endl;
    }
    
    return 0;
}
```

### Automatic Cache Management

```cpp
CurrencyConverter converter;

// This will fetch rates only if cache has expired (default: 60 minutes)
converter.updateRatesIfNeeded();

// Convert with fresh rates
double result = converter.convert(100.0, "USD", "GBP");
```

### Using Custom API

```cpp
CurrencyConverter converter;

// Set custom API endpoint
converter.setApiUrl("https://api.your-bank.com/v1/exchange-rates");

// If API requires authentication
converter.setApiKey("your-api-key-here");

// Fetch rates
converter.fetchRatesFromAPI("USD");
```

### Fetching Specific Currencies

```cpp
CurrencyConverter converter;

std::vector<std::string> currencies = {"EUR", "GBP", "JPY", "CAD"};
converter.fetchRatesForCurrencies(currencies, "USD");
```

### Manual Rate Updates (Still Supported)

```cpp
CurrencyConverter converter;

// Manual updates still work for testing or offline use
converter.updateRate("EUR", 0.92);
converter.updateRate("GBP", 0.79);

double result = converter.convert(100.0, "EUR", "GBP");
```

## Supported APIs

The default API is **exchangerate-api.com** which provides free exchange rates without authentication.

### Alternative APIs

You can use other APIs by setting a custom URL:

- **Fixer.io**: `https://api.fixer.io/latest`
- **CurrencyLayer**: `https://api.currencylayer.com/live`
- **Banking APIs**: Any REST API that returns JSON with exchange rates

### API Response Format

The converter supports multiple JSON response formats:

1. **Standard format**:
```json
{
  "base": "USD",
  "rates": {
    "EUR": 0.92,
    "GBP": 0.79,
    "JPY": 150.0
  }
}
```

2. **Alternative format**:
```json
{
  "conversion_rates": {
    "EUR": 0.92,
    "GBP": 0.79
  }
}
```

## Dependencies

- **cpr**: C++ Requests library (automatically fetched via CMake)
- **libcurl**: Required by cpr (must be installed on system)
- **nlohmann/json**: Already included in the project

### Installing libcurl

**Windows (MinGW)**:
```bash
# Using vcpkg
vcpkg install curl

# Or download from: https://curl.se/windows/
```

**Linux**:
```bash
sudo apt-get install libcurl4-openssl-dev  # Ubuntu/Debian
sudo yum install libcurl-devel            # CentOS/RHEL
```

**macOS**:
```bash
brew install curl
```

## Configuration

### Cache Timeout

```cpp
// Set cache to expire after 30 minutes
converter.cacheTimeout = std::chrono::minutes(30);
```

### Base Currency

```cpp
// Change base currency
converter.baseCurrency = "EUR";
converter.fetchRatesFromAPI("EUR");
```

## Error Handling

The API methods return `bool` to indicate success:

```cpp
if (!converter.fetchRatesFromAPI()) {
    std::cerr << "Failed to fetch rates. Using cached or default values." << std::endl;
    // Fallback to manual rates or cached values
    converter.updateRate("EUR", 0.92);  // Use fallback rate
}
```

## Thread Safety

The current implementation is **not thread-safe**. If using from multiple threads, add appropriate synchronization.

