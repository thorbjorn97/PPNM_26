// fspline.cc: quadratic spline (functional)

#include <functional>
#include <utility>

#include "fspline.h"

namespace pp {

// TASK C.1: Functional quadratic spline implementation with explicit capture of move-copies
std::function<double(double)>
make_qspline(vector x,
             vector y)
{
    int n = x.size();

    vector h(n-1);
    vector p(n-1);

    for(int i = 0; i < n-1; i++){
        h[i] = x[i+1] - x[i];
        p[i] = (y[i+1] - y[i]) / h[i];
    }

    // forward recursion
    vector c_f(n-1);
    c_f[0] = 0.0;

    for(int i = 0; i < n-2; i++){
        c_f[i+1] =
            (p[i+1] - p[i] - c_f[i]*h[i]) / h[i+1];
    }

    // backward recursion
    vector c_b(n-1);
    c_b[n-2] = 0.0;

    for(int i = n-3; i >= 0; i--){
        c_b[i] =
            (p[i+1] - p[i] - c_b[i+1]*h[i+1]) / h[i];
    }

    // final coefficients
    vector b(n-1), c(n-1);

    for(int i = 0; i < n-1; i++){
        c[i] = 0.5 * (c_f[i] + c_b[i]);
        b[i] = p[i] - c[i] * h[i];
    }

    return [x = std::move(x),
            y = std::move(y),
            b = std::move(b),
            c = std::move(c)]
    (double z)
    {
        int i = 0, j = x.size() - 1;

        while(j - i > 1){
            int mid = (i + j) / 2;
            if(z > x[mid]) i = mid;
            else j = mid;
        }

        double dx = z - x[i];
        return y[i] + b[i]*dx + c[i]*dx*dx;
    };
}

} // namespace pp
