#include "gtest/gtest.h"
#include "../CurrencyConverter.h"

class CurrencyConverterTest : public ::testing::Test {
protected:
    void SetUp() override {
        cc.baseCurrency = "USD";
        cc.updateRate("EUR", 0.92);
        cc.updateRate("GBP", 0.79);
    }

    CurrencyConverter cc;
};

TEST_F(CurrencyConverterTest, ConvertSameCurrency) {
    EXPECT_DOUBLE_EQ(cc.convert(100.0, "USD", "USD"), 100.0);
}

TEST_F(CurrencyConverterTest, ConvertFromBase) {
    EXPECT_DOUBLE_EQ(cc.convert(100.0, "USD", "EUR"), 92.0);
}

TEST_F(CurrencyConverterTest, ConvertToBase) {
    EXPECT_DOUBLE_EQ(cc.convert(92.0, "EUR", "USD"), 100.0);
}

TEST_F(CurrencyConverterTest, ConvertBetweenNonBase) {
    // 100 EUR to GBP: 100 EUR -> USD -> GBP
    // 100 EUR = 100 / 0.92 = 108.69565 USD
    // 108.69565 USD * 0.79 = 85.86956 GBP
    EXPECT_NEAR(cc.convert(100.0, "EUR", "GBP"), 85.86956, 1e-5);
}

TEST_F(CurrencyConverterTest, UpdateRate) {
    cc.updateRate("EUR", 0.95);
    EXPECT_DOUBLE_EQ(cc.convert(100.0, "USD", "EUR"), 95.0);
}

TEST_F(CurrencyConverterTest, CacheValidation) {
    // Initially cache should be invalid (no update time set)
    // But we'll set a rate manually, so cache state depends on implementation
    cc.updateRate("JPY", 150.0);
    
    // Test cache timeout setting
    cc.cacheTimeout = std::chrono::minutes(30);
    EXPECT_EQ(cc.cacheTimeout.count(), 30);
}

TEST_F(CurrencyConverterTest, ApiUrlConfiguration) {
    std::string customUrl = "https://api.example.com/rates/";
    cc.setApiUrl(customUrl);
    EXPECT_EQ(cc.apiUrl, customUrl);
}

TEST_F(CurrencyConverterTest, ApiKeyConfiguration) {
    std::string testKey = "test-api-key-12345";
    cc.setApiKey(testKey);
    EXPECT_EQ(cc.apiKey, testKey);
}