#include <cmath>
#include <tuple>
#include <algorithm>
#include <functional>
#include <vector>
#include <limits>
#include <cstdio>

#include "ode.h"

namespace pp {

// ============================================================================
// RKF45 stepper
// ============================================================================
std::tuple<vector, vector>
rkstep45(
    std::function<vector(double, vector)> F,
    double x,
    vector y,
    double h
){
    // Runge-Kutta-Fehlberg 4(5) Coefficients
    vector k1 = F(x, y);
    vector k2 = F(x + 1.0/4.0 * h, y + k1 * (1.0/4.0 * h));
    vector k3 = F(x + 3.0/8.0 * h, y + k1 * (3.0/32.0 * h) + k2 * (9.0/32.0 * h));
    vector k4 = F(x + 12.0/13.0 * h, y + k1 * (1932.0/2197.0 * h) - k2 * (7200.0/2197.0 * h) + k3 * (7296.0/2197.0 * h));
    vector k5 = F(x + h, y + k1 * (439.0/216.0 * h) - k2 * (8.0 * h) + k3 * (3680.0/513.0 * h) - k4 * (845.0/4104.0 * h));
    vector k6 = F(x + 1.0/2.0 * h, y - k1 * (8.0/27.0 * h) + k2 * (2.0 * h) - k3 * (3544.0/2565.0 * h) + k4 * (1859.0/4104.0 * h) - k5 * (11.0/40.0 * h));

    // 4th-order estimate
    vector y4 = y + k1 * (25.0/216.0 * h) + k3 * (1408.0/2565.0 * h) + k4 * (2197.0/4104.0 * h) - k5 * (1.0/5.0 * h);
    
    // 5th-order estimate
    vector y5 = y + k1 * (16.0/135.0 * h) + k3 * (6656.0/12825.0 * h) + k4 * (28561.0/56430.0 * h) - k5 * (9.0/50.0 * h) + k6 * (2.0/55.0 * h);

    vector err = y5 - y4;
    return {y5, err}; // Return higher order estimate for local extrapolation
}


// ============================================================================
// DRIVER
// ============================================================================
std::tuple<std::vector<double>, std::vector<vector>>
driver(
    std::function<vector(double, vector)> F,
    double a,
    double b,
    vector yinit,
    double h,
    double acc,
    double eps
){
    double x = a;
    vector y = yinit;

    std::vector<double> xlist;
    std::vector<vector> ylist;

    xlist.push_back(x);
    ylist.push_back(y);

    const double SAFETY = 0.95;
    const double POWER  = 0.20;   // RKF45 optimal exponent = 1/5
    const double H_MIN  = 1e-10;
    const double H_MAX  = (b - a) / 2.0;
    const double H_GEOM = (b - a) / 500.0;

    while (x < b) {

        if (x + h > b) h = b - x;

        auto [yh, err_vec] = rkstep45(F, x, y, h);

        double err = err_vec.norm();
        double tol = (acc + eps * yh.norm()) * std::sqrt(std::abs(h) / std::abs(b - a));
                    
        if (err <= tol) {
            x += h;
            y = yh;
            xlist.push_back(x);
            ylist.push_back(y);
        }

        // -------------------------------
        // STEP SIZE CONTROL (STABLE)
        // -------------------------------
        if (err > 0.0) {
            double factor = SAFETY * std::pow(tol / err, POWER);
            factor = std::clamp(factor, 0.2, 2.0);

            h *= factor;
        } else {
            h *= 2.0;
        }

        // HARD SAFEGUARDS
        h = std::clamp(h, H_MIN, H_MAX);

        // Enforce minimum resolution
        h = std::min(h, H_GEOM);

        // Prevent stagnation
        if (std::abs(h) < H_MIN) {
            std::fprintf(stderr,
                "Warning: step size collapsed at x = %.16g\n", x);
            break;
        }
    }

    return {xlist, ylist};
}

} // namespace pp
