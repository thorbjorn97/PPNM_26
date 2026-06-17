// erf.cc
// Implementation of the error function via its integral representation (Task A)

#include <cmath>

#include "integrator.h"
#include "erf.h"

double erf_integral(double z, double acc, double eps)
{
    const double factor = 2.0 / std::sqrt(M_PI);
     
    // Case 1: z < 0
    // Use odd symmetry of erf:
    // erf(-z) = -erf(z)
    if (z < 0.0)
    {
        return -erf_integral(-z, acc, eps);
    }

    // Case 2: 0 <= z <= 1
    // Use direct definition:
    // erf(z) = (2/√π) * ∫_0^z dx exp(-x²)
    if (z <= 1.0)
    {
        // Define basic Gaussian function: f(x) = exp(-x²)
        auto f = [](double x)
        {
            return std::exp(-x*x);
        };
        // Numerically integrate on [0, z] using adaptive integrator
        return factor * integrate(f, 0.0, z, acc, eps).value;
    }

    // Case 3: 1 < z
    // Use transformed integral to avoid poor convergence of Gaussian tail:
    // erf(z) = 1 - (2/√π) * ∫_0^1 dt exp(-(z + (1-t)/t)²) / t²
    auto g = [z](double t)
    {
        // Guard against division by zero or numbers near zero
        if (t < 1e-15) return 0.0; 
        
        double x = z + (1.0 - t) / t;
        
        // Prevent floating-point overflow inside std::exp
        if (x > 40.0) return 0.0; 
        
        return std::exp(-x*x) / (t*t);
    };        
    // Numerically integrate on [0, 1] using adaptive integrator
    return 1.0 - factor * integrate(g, 0.0, 1.0, acc, eps).value;
}