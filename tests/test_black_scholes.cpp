
#include <gtest/gtest.h>

#include "blackscholes/black_scholes.hpp"

#include <cmath>
#include <stdexcept>

using namespace blackscholes;


// TEST 1: European call price
TEST(BlackScholesTests, EuropeanCallPrice) {

    EuropeanOption call{
        OptionType::Call, 100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    const double result = price(call);

    EXPECT_NEAR(result, 10.4506, 0.0001);
}


// TEST 2: European put price
TEST(BlackScholesTests, EuropeanPutPrice) {

    EuropeanOption put{
        OptionType::Put, 100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    const double result = price(put);

    EXPECT_NEAR(result, 5.5735, 0.0001);
}


// TEST 3: Put-call parity
TEST(BlackScholesTests, PutCallParity) {

    EuropeanOption call{
        OptionType::Call, 100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    EuropeanOption put{
        OptionType::Put, 100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    const double call_price = price(call);
    const double put_price = price(put);

    const double lhs = call_price - put_price;
    const double rhs = 100.0 - 100.0 * std::exp(-0.05);

    EXPECT_NEAR(lhs, rhs, 1e-10);
}


// TEST 4: Invalid spot price
TEST(BlackScholesTests, RejectsInvalidSpotPrice) {

    EuropeanOption option{
        OptionType::Call, -100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    EXPECT_THROW(price(option), std::invalid_argument);
}


// TEST 5: Invalid volatility
TEST(BlackScholesTests, RejectsInvalidVolatility) {

    EuropeanOption option{
        OptionType::Call, 100.0, 100.0, 1.0, 0.05, 0.0, 0.0
    };

    EXPECT_THROW(price(option), std::invalid_argument);
}
