// test_functions.cc

#include "test_functions.h"

namespace pp {

double unit_circle(const vector& x){  // simple unit circle (Task A.2)

    return (x[0]*x[0] + x[1]*x[1] <= 1.0)
        ? 1.0
        : 0.0;
}


double gaussian(const vector& x){  // simple Gaussian (Task A.2)
    return std::exp(-(x[0]*x[0] + x[1]*x[1]));
}

double ellipsoid(const vector& x){  // simple ellipsoid (Task A.2)

    const double q =
          x[0]*x[0]
        + x[1]*x[1]/4.0
        + x[2]*x[2]/9.0;

    return (q <= 1.0)
        ? 1.0
        : 0.0;
}

double smooth_test(const vector& x) { // simple smooth 3D test (Task B.2)
    return x[0] * x[1] * x[2]; 
}

double singular_integrand(const vector& x) { // difficult singular integrand (Task B.3)
    double cx = std::cos(x[0]);
    double cy = std::cos(x[1]);
    double cz = std::cos(x[2]);

    double denom = (1.0 - cx * cy * cz) * M_PI * M_PI * M_PI;
    return 1.0 / denom;
}

} // namespace pp
