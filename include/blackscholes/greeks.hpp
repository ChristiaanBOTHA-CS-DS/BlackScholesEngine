#pragma once

#include "option.hpp"

namespace blackscholes {
    struct Greeks {

        double delta;
        double gamma;
        double theta;
        double vega;
        double rho;

    };

    Greeks calculate_greeks(const EuropeanOption& option);

}