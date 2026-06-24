// cspline.cc: cubic spline (OOP)

#include <cassert>

#include "cspline.h"

namespace pp {

// cubic spline implementation using OOP with derivative and definite integral
cspline::cspline(const vector& x_in,
                 const vector& y_in)
    : n(static_cast<int>(x_in.size())),
      x(x_in),
      y(y_in),
      b(n-1),
      c(n),
      d(n-1)
{
    vector h(n-1);
    vector alpha(n-1);

    // step sizes and slopes
    for (int i = 0; i < n - 1; i++) {
        h[i] = x[i+1] - x[i];
        assert(h[i] > 0);

        alpha[i] = (y[i+1] - y[i]) / h[i];
    }

    // system variables
    vector l(n), mu(n), z(n);

    l[0] = 1.0;
    mu[0] = 0.0;
    z[0] = 0.0;

    // forward sweep
    for (int i = 1; i < n - 1; i++) {
        l[i] = 2.0 * (x[i+1] - x[i-1]) - h[i-1] * mu[i-1];
        mu[i] = h[i] / l[i];
        z[i] = (3.0 * (alpha[i] - alpha[i-1]) - h[i-1] * z[i-1]) / l[i];
    }

    l[n-1] = 1.0;
    z[n-1] = 0.0;
    c[n-1] = 0.0;

    // backward substitution
    for (int j = n - 2; j >= 0; j--) {
        c[j] = z[j] - mu[j] * c[j+1];
        b[j] = alpha[j] - h[j] * (c[j+1] + 2.0 * c[j]) / 3.0;
        d[j] = (c[j+1] - c[j]) / (3.0 * h[j]);
    }
}

// binary search for the correct interval
int cspline::binsearch(double z) const {
    int i = 0;
    int j = n - 1;

    while (j - i > 1) {
        int mid = (i + j) / 2;
        if (z > x[mid]) i = mid;
        else j = mid;
    }
    return i;
}

// evaluate the cubic spline at a point z
double cspline::eval(double z) const {
    int i = binsearch(z);
    double dx = z - x[i];

    return y[i]
         + b[i] * dx
         + c[i] * dx * dx
         + d[i] * dx * dx * dx;
}

// evaluate the derivative of the cubic spline at a point z
double cspline::deriv(double z) const {
    int i = binsearch(z);
    double dx = z - x[i];

    return b[i]
         + 2.0 * c[i] * dx
         + 3.0 * d[i] * dx * dx;
}

// evaluate the definite integral of the cubic spline from x[0] to a point z
double cspline::integ(double z) const {
    int idx = binsearch(z);

    double sum = 0.0;

    for (int i = 0; i < idx; i++) {
        double h = x[i+1] - x[i];

        sum += y[i]*h
             + 0.5*b[i]*h*h
             + (1.0/3.0)*c[i]*h*h*h
             + 0.25*d[i]*h*h*h*h;
    }

    double dx = z - x[idx];

    sum += y[idx]*dx
         + 0.5*b[idx]*dx*dx
         + (1.0/3.0)*c[idx]*dx*dx*dx
         + 0.25*d[idx]*dx*dx*dx*dx;

    return sum;
}

} // namespace pp
