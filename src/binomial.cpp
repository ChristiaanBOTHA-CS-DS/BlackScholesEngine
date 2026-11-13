
#include "blackscholes/binomial.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace blackscholes {

    double price_binomial(
        const EuropeanOption& option,
        unsigned int steps
    ) {

        const double S = option.spot_price;
        const double K = option.strike_price;
        const double T = option.time_to_maturity;
        const double r = option.risk_free_rate;
        const double sigma = option.volatility;
        const double q = option.dividend_yield;

        // Validate inputs
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

        if (steps == 0) {
            throw std::invalid_argument(
                "Number of steps must be positive."
            );
        }

        if (option.exercise_style != ExerciseStyle::European
            && option.exercise_style != ExerciseStyle::American) {

            throw std::invalid_argument(
                "Invalid exercise style."
            );
        }

        if (option.type != OptionType::Call
            && option.type != OptionType::Put) {

            throw std::invalid_argument(
                "Invalid option type."
            );
        }

        // Calculate the length of each time step
        const double dt = T / steps;

        // CRR up and down factors
        const double u = std::exp(sigma * std::sqrt(dt));
        const double d = 1.0 / u;

        // Risk-neutral probability
        const double p =
            (std::exp((r - q) * dt) - d) / (u - d);

        if (!std::isfinite(p) || p <= 0.0 || p >= 1.0) {
            throw std::invalid_argument(
                "Invalid risk-neutral probability."
            );
        }

        // Discount factor
        const double discount = std::exp(-r * dt);

        // Store option values at maturity
        std::vector<double> values(steps + 1);

        // Calculate terminal payoffs
        for (unsigned int j = 0; j <= steps; ++j) {

            const double stock_price =
                S * std::pow(u, j) * std::pow(d, steps - j);

            if (option.type == OptionType::Call) {
                values[j] = std::max(stock_price - K, 0.0);
            }
            else {
                values[j] = std::max(K - stock_price, 0.0);
            }
        }

        // Backward induction
        for (int i = static_cast<int>(steps) - 1; i >= 0; --i) {

            for (int j = 0; j <= i; ++j) {

                // Calculate continuation value
                const double continuation_value =
                    discount * (
                        p * values[j + 1]
                        + (1.0 - p) * values[j]
                    );

                // European options can only be exercised at maturity
                if (option.exercise_style == ExerciseStyle::European) {

                    values[j] = continuation_value;

                }
                // American options can be exercised at any node
                else {

                    const double stock_price =
                        S * std::pow(u, j) * std::pow(d, i - j);

                    double exercise_value;

                    if (option.type == OptionType::Call) {
                        exercise_value =
                            std::max(stock_price - K, 0.0);
                    }
                    else {
                        exercise_value =
                            std::max(K - stock_price, 0.0);
                    }

                    values[j] = std::max(
                        exercise_value,
                        continuation_value
                    );
                }
            }
        }

        // Return the option value at the root node
        return values[0];
    }

}
