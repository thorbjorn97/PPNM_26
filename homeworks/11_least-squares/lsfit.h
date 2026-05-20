//lsfit.h

#pragma once

#include <vector>
#include <functional>

#include "vector.h"
#include "matrix.h"
#include "qr.h"

namespace pp {

// Fit result container
struct FitResult {
    vector c;
    matrix cov;
};

// Task A: OLS fit using QR
vector lsfit(
    const std::vector<std::function<double(double)>>& fs,
    const vector& x,
    const vector& y,
    const vector& dy
);

// Task B: OLS fit using QR including covariance estimation
FitResult lsfit_mod(
    const std::vector<std::function<double(double)>>& fs,
    const vector& x,
    const vector& y,
    const vector& dy
);

} // namespace pp