// qspline.cc: quadratic spline (OOP)

#include <cassert>

#include "qspline.h"

namespace pp {

qspline::qspline(const vector& x_in,
                 const vector& y_in)
        : n(x_in.size()),
          x(x_in),
          y(y_in),
          b(n-1),
          c(n-1)
{
    vector h(n-1);
    vector p(n-1);

    for(int i = 0; i < n-1; i++){
        h[i] = x[i+1] - x[i];
        p[i] = (y[i+1] - y[i]) / h[i];
    }

    // Forward recursion (c_f)
    vector c_f(n-1);
    c_f[0] = 0.0;

    for(int i = 0; i < n-2; i++){
        c_f[i+1] =
            (p[i+1] - p[i] - c_f[i]*h[i]) / h[i+1];
    }

    // Backward recursion (c_b)
    vector c_b(n-1);
    c_b[n-2] = 0.0;

    for(int i = n-3; i >= 0; i--){
        c_b[i] =
            (p[i+1] - p[i] - c_b[i+1]*h[i+1]) / h[i];
    }

    // Average
    for(int i = 0; i < n-1; i++){
        c[i] = 0.5 * (c_f[i] + c_b[i]);
        b[i] = p[i] - c[i] * h[i];
    }
}

int qspline::binsearch(double z) const {

    int i = 0;
    int j = n - 1;

    while(j - i > 1){
        int mid = (i + j) / 2;

        if(z > x[mid])
            i = mid;
        else
            j = mid;
    }

    return i;
}

double qspline::eval(double z) const {

    int i = binsearch(z);
    double dx = z - x[i];

    return y[i]
         + b[i]*dx
         + c[i]*dx*dx;
}

double qspline::deriv(double z) const {

    int i = binsearch(z);
    double dx = z - x[i];

    return b[i] + 2.0*c[i]*dx;
}

double qspline::integ(double z) const {

    int i = binsearch(z);

    double sum = 0.0;

    for(int k=0; k<i; k++){
        double h = x[k+1] - x[k];

        sum += y[k]*h
             + 0.5*b[k]*h*h
             + (1.0/3.0)*c[k]*h*h*h;
    }

    double dx = z - x[i];

    sum += y[i]*dx
         + 0.5*b[i]*dx*dx
         + (1.0/3.0)*c[i]*dx*dx*dx;

    return sum;
}

} // namespace pp
