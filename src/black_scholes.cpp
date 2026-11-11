#include "blackscholes/black_scholes.hpp"

#include <cmath>
#include <stdexcept>

namespace blackscholes {

    // Standard Normal cumulative distribution function
    double normal_cdf(double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    }

    //Black Scholes pricing function
    double price(const EuropeanOption& option) {

        const double S = option.spot_price;
        const double K = option.strike_price;
        const double T= option.time_to_maturity;
        const double r = option.risk_free_rate;
        const double sigma = option.volatility;
        const double q = option.dividend_yield;

        //Validate inputs
        if (!std::isfinite(S) || !std::isfinite(K) || !std::isfinite(T) || !std::isfinite(r) || !std::isfinite(sigma) || !std::isfinite(q)) {

            throw std::invalid_argument("Option inputs must be finite.");
        }

        if (S <= 0.0 || K <= 0.0 || T <= 0.0 || sigma <= 0.0) {
            throw std::invalid_argument("Spot, strike, maturity and volatility must be positive");
        }

        //Calculate d1 and d2

        const double sqrt_T = std::sqrt(T);

        const double d1 = (std::log(S / K) + (r - q + 0.5 * sigma * sigma) * T) / (sigma * sqrt_T);
        const double d2 = d1 - (sigma * sqrt_T);

        //Discount factors
        const double discount_stock = std::exp(-q * T);
        const double discount_strike = std::exp(-r * T);

        //Calculate Option Price
        if (option.type == OptionType::Call) {

            return S * discount_stock * normal_cdf(d1) - K * discount_strike * normal_cdf(d2);
            
        }

        if (option.type == OptionType::Put) {
            return K* discount_strike * normal_cdf(-d2) - S * discount_stock * normal_cdf(-d1);
        }

        throw std::invalid_argument("Invalid Option Type");
    }
}