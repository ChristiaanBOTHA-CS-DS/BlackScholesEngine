#pragma once

#include "option.hpp"

namespace blackscholes {

    double price_binomial(
        const EuropeanOption& option,
        unsigned int steps
    );

}