#include "blackscholes/greeks.hpp"

#include <cmath>
#include <numbers>
#include <stdexcept>

namespace blackscholes {
    
    namespace {

        //Standard normal cumulative distribution function
        double normal_cdf(double x) {
            return 0.5 * std::erfc(-x / std::sqrt(2.0));
        }

        //Standard normal density function
        double normal_pdf(double x) {
            return std::exp(-0.5 * x * x) / std::sqrt(2.0 * std::numbers::pi);
        }

    }

    Greeks calculate_greeks(const EuropeanOption& option) {

        const double S = option.spot_price;
        const double K = option.strike_price;
        const double T = option.time_to_maturity;
        const double r = option.risk_free_rate;
        const double sigma = option.volatility;
        const double q = option.dividend_yield;

        //Validate Inputs
        if (!std::isfinite(S) || !std::isfinite(K)
            || !std::isfinite(T) || !std::isfinite(r)
            || !std::isfinite(sigma) || !std::isfinite(q)) {

            throw std::invalid_argument(
                "Option inputs must be finite."
            );
        }

        if (S <= 0.0 || K <= 0.0 || T <= 0.0 || sigma <= 0.0) {
            throw std::invalid_argument(
                "Spot, strike, maturity and volatility must be positive."
            );
        }

        //Calculate d1 and d2
        const double sqrt_T = std::sqrt(T);

        const double d1 =
            (std::log(S / K)
            + (r - q + 0.5 * sigma * sigma) * T)
            / (sigma * sqrt_T);

        const double d2 = d1 - sigma * sqrt_T;

        // Discount factors
        const double discount_stock = std::exp(-q * T);
        const double discount_strike = std::exp(-r * T);

        // Normal distribution values
        const double N_d1 = normal_cdf(d1);
        const double N_d2 = normal_cdf(d2);
        const double N_minus_d1 = normal_cdf(-d1);
        const double N_minus_d2 = normal_cdf(-d2);

        const double phi_d1 = normal_pdf(d1);

        // Calculate Gamma and Vega
        const double gamma =
            discount_stock * phi_d1
            / (S * sigma * sqrt_T);

        const double vega =
            S * discount_stock * phi_d1 * sqrt_T;

        // Calculate Delta, Theta and Rho
        double delta;
        double theta;
        double rho;

        if (option.type == OptionType::Call) {

            delta = discount_stock * N_d1;

            theta =
                -S * discount_stock * phi_d1 * sigma
                    / (2.0 * sqrt_T)
                - r * K * discount_strike * N_d2
                + q * S * discount_stock * N_d1;

            rho =
                K * T * discount_strike * N_d2;

        }
        else if (option.type == OptionType::Put) {

            delta = discount_stock * (N_d1 - 1.0);

            theta =
                -S * discount_stock * phi_d1 * sigma
                    / (2.0 * sqrt_T)
                + r * K * discount_strike * N_minus_d2
                - q * S * discount_stock * N_minus_d1;

            rho =
                -K * T * discount_strike * N_minus_d2;

        }
        else {
            throw std::invalid_argument("Invalid Option Type.");
        }

        return Greeks{
            delta,
            gamma,
            theta,
            vega,
            rho
        };

    }

}