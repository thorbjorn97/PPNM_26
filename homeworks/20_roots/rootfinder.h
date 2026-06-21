// rootfinder.h

#pragma once

#include <stdexcept>
#include <cstddef>
#include <cmath>
#include <functional>

#include "matrix.h"
#include "vector.h"

namespace pp {

matrix jacobian(
    const std::function<vector(const vector&)>& f,
    const vector& x,
    const vector& fx,
    vector dx
);

// Task A (Newton's method with simple back-tracking line-search)
struct newton_result {
    vector root;
    std::string exit_reason;
    int iterations_taken;
};

newton_result newton(
    const std::function<vector(const vector&)>& f,
    vector x0,
    double acc = 1e-6,
    double alpha_min = 1e-6,
    int max_iter = 1000,
    vector dx = vector(0)
);

// Task C (Broyden updates + Quadratic interpolation line-search)
newton_result newton_broyden_quadratic(
    std::function<vector(const vector&)> f,
    vector x0,
    double acc = 1e-6,
    double alpha_min = 1e-6,
    int max_iter = 1000
);

}
