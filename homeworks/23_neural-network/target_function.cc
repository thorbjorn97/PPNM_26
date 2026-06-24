// target_function.cc

#include <cmath>

#include "target_function.h"

namespace pp {

// Target function to be interpolated (as per instruction suggestion)
double g(double x) {
    return std::cos(5.0 * x - 1.0) * std::exp(-x * x);
}

// Exact analytical first derivative
double g_derivative(double x) {
    double e_term = std::exp(-x * x);
    return -(5.0 * std::sin(5.0 * x - 1.0) + 2.0 * x * std::cos(5.0 * x - 1.0)) * e_term;
}

// Exact analytical second derivative
double g_second_derivative(double x) {
    double e_term = std::exp(-x * x);
    double cos_term = (4.0 * x * x - 27.0) * std::cos(5.0 * x - 1.0);
    double sin_term = 20.0 * x * std::sin(5.0 * x - 1.0);
    return (cos_term + sin_term) * e_term;
}

} // namespace pp
