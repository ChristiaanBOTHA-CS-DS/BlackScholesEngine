
#include <gtest/gtest.h>

#include "blackscholes/black_scholes.hpp"
#include "blackscholes/greeks.hpp"
#include "blackscholes/binomial.hpp"

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


// Verify analytical Greeks against numerical finite differences
void VerifyGreeksMatchFiniteDifferences(
    const EuropeanOption& option
) {

    const Greeks analytical = calculate_greeks(option);

    // Small perturbations
    const double hS = 0.01;
    const double hT = 0.0001;
    const double hSigma = 0.0001;
    const double hr = 0.0001;

    // Create perturbed options
    EuropeanOption S_up = option;
    EuropeanOption S_down = option;

    S_up.spot_price += hS;
    S_down.spot_price -= hS;

    EuropeanOption T_up = option;
    EuropeanOption T_down = option;

    T_up.time_to_maturity += hT;
    T_down.time_to_maturity -= hT;

    EuropeanOption sigma_up = option;
    EuropeanOption sigma_down = option;

    sigma_up.volatility += hSigma;
    sigma_down.volatility -= hSigma;

    EuropeanOption r_up = option;
    EuropeanOption r_down = option;

    r_up.risk_free_rate += hr;
    r_down.risk_free_rate -= hr;

    // Numerical Delta
    const double numerical_delta =
        (price(S_up) - price(S_down)) / (2.0 * hS);

    // Numerical Gamma
    const double numerical_gamma =
        (price(S_up) - 2.0 * price(option) + price(S_down))
        / (hS * hS);

    // Numerical Theta
    const double numerical_theta =
        -(price(T_up) - price(T_down)) / (2.0 * hT);

    // Numerical Vega
    const double numerical_vega =
        (price(sigma_up) - price(sigma_down))
        / (2.0 * hSigma);

    // Numerical Rho
    const double numerical_rho =
        (price(r_up) - price(r_down))
        / (2.0 * hr);

    // Compare analytical and numerical results
    EXPECT_NEAR(analytical.delta, numerical_delta, 1e-5);
    EXPECT_NEAR(analytical.gamma, numerical_gamma, 1e-5);
    EXPECT_NEAR(analytical.theta, numerical_theta, 1e-5);
    EXPECT_NEAR(analytical.vega, numerical_vega, 1e-5);
    EXPECT_NEAR(analytical.rho, numerical_rho, 1e-5);
}


// TEST 6: Call Greeks
TEST(BlackScholesTests, CallGreeksMatchFiniteDifferences) {

    EuropeanOption call{
        OptionType::Call,
        100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    VerifyGreeksMatchFiniteDifferences(call);
}


// TEST 7: Put Greeks
TEST(BlackScholesTests, PutGreeksMatchFiniteDifferences) {

    EuropeanOption put{
        OptionType::Put,
        100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    VerifyGreeksMatchFiniteDifferences(put);
}


 
// TEST 8: Binomial call price converges to Black-Scholes price
TEST(BlackScholesTests, BinomialCallConvergesToBlackScholes) {

    EuropeanOption call{
        OptionType::Call,
        100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    const double analytical_price = price(call);
    const double binomial_price = price_binomial(call, 1000);

    EXPECT_NEAR(binomial_price, analytical_price, 0.01);
}


// TEST 9: Binomial put price converges to Black-Scholes price
TEST(BlackScholesTests, BinomialPutConvergesToBlackScholes) {

    EuropeanOption put{
        OptionType::Put,
        100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    const double analytical_price = price(put);
    const double binomial_price = price_binomial(put, 1000);

    EXPECT_NEAR(binomial_price, analytical_price, 0.01);
}


// TEST 10: Binomial put-call parity
TEST(BlackScholesTests, BinomialPutCallParity) {

    EuropeanOption call{
        OptionType::Call,
        100.0, 100.0, 1.0, 0.05, 0.20, 0.02
    };

    EuropeanOption put{
        OptionType::Put,
        100.0, 100.0, 1.0, 0.05, 0.20, 0.02
    };

    const double call_price = price_binomial(call, 200);
    const double put_price = price_binomial(put, 200);

    const double lhs = call_price - put_price;

    const double rhs =
        100.0 * std::exp(-0.02)
        - 100.0 * std::exp(-0.05);

    EXPECT_NEAR(lhs, rhs, 1e-9);
}


// TEST 11: Reject zero binomial steps
TEST(BlackScholesTests, RejectsZeroBinomialSteps) {

    EuropeanOption call{
        OptionType::Call,
        100.0, 100.0, 1.0, 0.05, 0.20, 0.0
    };

    EXPECT_THROW(
        price_binomial(call, 0),
        std::invalid_argument
    );
}

