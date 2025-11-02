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