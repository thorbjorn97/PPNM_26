// minimizer.h

#pragma once

#include <functional>

#include "vector.h"
#include "matrix.h"

namespace pp {

// Structure to hold the minimizer result
struct minimizer_result {
    vector min_point;
    int steps_taken;
};

// Numerical gradient calculation with baseline evaluation caching
vector gradient(
    const std::function<double(const vector&)>& phi,
    const vector& x,
    double phi_x
);
// ^Optimized: Receives cached phi(x) to save 1 function evaluation per call

// Numerical Hessian calculation using finite differences of the gradient
matrix hessian(
    const std::function<double(const vector&)>& phi,
    const vector& x,
    const vector& g_x
);
// ^Optimized: Receives the already-computed gradient vector to save (1 + n) evaluations

// Newton's minimization method with numerical gradient, numerical Hessian and backtracking line search
minimizer_result minimize_newton(
    const std::function<double(const vector&)>& phi,
    const vector& x_start,
    double acc = 1e-3,
    int max_iter = 1000,
    const std::string& log_filename = "" // argument for logging
);

} // namespace pp