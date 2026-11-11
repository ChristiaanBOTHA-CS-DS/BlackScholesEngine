#pragma once

namespace blackscholes {
    enum class OptionType {
        Call,
        Put
    };

    struct EuropeanOption {
        OptionType type;
        
        double spot_price;
        double strike_price;
        double time_to_maturity;

        double risk_free_rate;
        double volatility;
        double dividend_yield = 0.0;
    };
};