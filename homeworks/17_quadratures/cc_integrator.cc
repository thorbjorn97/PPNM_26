// cc_integrator.cc

#include <cmath>

#include "cc_integrator.h"
#include "integrator.h"

// Clenshaw-Curtis integrator for finite intervals [a, b]
IntegrationResult integrate_cc(
    std::function<double(double)> f,
    double a,
    double b,
    double acc,
    double eps)
{
    auto g = [f,a,b](double theta)
    {
        double x = (a+b)/2.0 + (b-a)/2.0*std::cos(theta);
        double jac = std::sin(theta) * (b-a)/2.0;
        return f(x)*jac;
    };

    return integrate(g, 0.0, M_PI, acc, eps);
}

// Generalization of Clenshaw-Curtis integrator to accept infinite limits (-inf, +inf) 
// using Equation (61) from the lecture notes
IntegrationResult integrate_cc_inf(
    std::function<double(double)> f,
    double acc,
    double eps)
{
    // Map (-inf, +inf) to (-1, 1) using Eq. (61)
    auto g = [f](double t)
    {
        // Guard against exact endpoints where 1 - t^2 == 0
        if (std::abs(t) >= 1.0 - 1e-15) return 0.0;

        double t2 = t * t;
        double denom = 1.0 - t2;
        
        double x = t / denom;
        double dxdt = (1.0 + t2) / (denom * denom);

        return f(x) * dxdt;
    };

    // Forward the finite [-1, 1] integral to Clenshaw-Curtis routine
    return integrate_cc(g, -1.0, 1.0, acc, eps);
}