// cc_integrator.h [From homework 17_quadratures]

#pragma once

#include <functional>

#include "integrator.h"

IntegrationResult integrate_cc(
    std::function<double(double)> f,
    double a,
    double b,
    double acc = 1e-6,
    double eps = 1e-6
);

IntegrationResult integrate_cc_inf(
    std::function<double(double)> f,
    double acc = 1e-6,
    double eps = 1e-6
);
