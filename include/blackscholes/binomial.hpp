#pragma once

#include "option.hpp"

#include <vector>

namespace blackscholes {

    struct EarlyExerciseBoundaryPoint {
        unsigned int step;
        double time;
        double stock_price;
    };

    double price_binomial(
        const Option& option,
        unsigned int steps
    );

    std::vector<EarlyExerciseBoundaryPoint>
    calculate_early_exercise_boundary(
        const Option& option,
        unsigned int steps
    );

}