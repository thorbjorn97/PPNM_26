// integrator_mc.cc

#include <cmath>

#include "integrator_mc.h"

namespace pp {

// Plain Monte Carlo multi-dimensional integrator
mc_result plain_mc(
    const std::function<double(const vector&)>& f,  // function to be integrated
    const vector& a,                // lower limits
    const vector& b,                // upper limits
    int N,                          // number of samples
    std::function<double()> rng     // random number generator, returns a double in [0,1)
){
    const size_t dim = a.size();    // dimension of the integration domain

    double V = 1.0;                 // volume of the integration domain (initially set to 1.0)
    
    // Compute actual volume as the side length product of the hyper-rectangle defined by a and b:
    for(size_t i = 0; i < dim; i++) // for each dimension
        V *= b[i]-a[i];             // accumulate the product of the side lengths to get the total volume

    double sum1 = 0.0;              // sum of f(x) over the samples
    double sum2 = 0.0;              // sum of f(x)^2 over the samples

    vector x(dim);                  // vector to hold the random point in the integration domain

    // Monte Carlo sampling loop
    for(int n = 0; n < N; n++){               // for each sample
        for(size_t i = 0; i < dim; i++)       // for each dimension
            x[i] = a[i] + rng()*(b[i]-a[i]);  // generate a random point in the integration domain
        const double fx = f(x);               // evaluate the function at the random point
        sum1 += fx;                           // accumulate the sum of f(x)
        sum2 += fx*fx;                        // accumulate the sum of f(x)^2
    }

    // Compute the mean and standard deviation of the function values
    const double mean  = sum1/N;
    const double sigma = std::sqrt(sum2/N - mean*mean);

    // Return estimated integral and estimated error
    return {
        mean * V,
        sigma * V / std::sqrt((double)N)  // convert N to double to avoid integer division
    };
}

} // namespace pp
