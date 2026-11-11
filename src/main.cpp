#include <iostream>
#include <iomanip>
#include <cmath>

#include "blackscholes/black_scholes.hpp"

int main() {

    using namespace blackscholes;

    //Define a European Call Option
    EuropeanOption call {
        OptionType::Call,
        100.0,   //Spot Price
        100.0,   //Strike Price
        1.0,     //Time To Maturity
        0.05,    //Risk-free Rate
        0.20,    //Volatility
        0.0,     //Divident Yield
    };

    //Define a European Put Option
    EuropeanOption put{

        OptionType::Put,
        100.0,   //Spot Price
        100.0,   //Strike Price
        1.0,     //Time To Maturity
        0.05,    //Risk-free Rate
        0.20,    //Volatility
        0.0,     //Divident Yield
    };

    //Calculate Prices
    const double call_price = price(call);
    const double put_price = price(put);

    //Verify put-call parity

    const double lhs = call_price - put_price;

    const double rhs = call.spot_price * std::exp(-call.dividend_yield * call.time_to_maturity) - (call.strike_price * std::exp(-call.risk_free_rate * call.time_to_maturity));

    const double parity_error = std::abs(lhs - rhs);

    const bool parity_holds = parity_error < 1e-10;

    //Display Results
    std::cout << "=================================\n";
    std::cout << "       BLACKSCHOLES ENGINE       \n";
    std::cout << "=================================\n\n";

    std::cout << std::fixed << std::setprecision(4);

    std::cout << "European Option Pricing\n\n";

    std::cout << "Call Price: " << call_price << '\n';
    std::cout << "Put Price:  " << put_price << '\n';

    std::cout << "\n=================================\n";

    std::cout << "\nPut-Call Parity Verification\n\n";

    std::cout << "Left-hand side:  " << lhs << '\n';
    std::cout << "Right-hand side: " << rhs << '\n';
    std::cout << "Absolute error:  " << parity_error << '\n';

    if (parity_holds) {
        std::cout << "Result: PASSED\n";
    }
    else {
        std::cout << "Result: FAILED\n";
    }

    return 0;

}