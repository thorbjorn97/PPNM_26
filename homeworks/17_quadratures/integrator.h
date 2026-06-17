// integrator.h

#pragma once

#include <functional>

struct IntegrationResult {
    double value;
    double error;
};

IntegrationResult integrate(
    const std::function<double(double)>& f,
    double a,
    double b,
    double acc = 1e-6,
    double eps = 1e-6
);

int get_integration_calls();