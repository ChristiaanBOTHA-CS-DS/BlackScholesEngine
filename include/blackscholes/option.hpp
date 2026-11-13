#pragma once

namespace blackscholes {

    enum class OptionType {
        Call,
        Put
    };

    enum class ExerciseStyle {
        European,
        American
    };

    struct Option {

        OptionType type;
        ExerciseStyle exercise_style = ExerciseStyle::European;

        double spot_price;
        double strike_price;
        double time_to_maturity;

        double risk_free_rate;
        double volatility;
        double dividend_yield = 0.0;

    };

    //Backward compatibility with existing code
    using EuropeanOption = Option;

}