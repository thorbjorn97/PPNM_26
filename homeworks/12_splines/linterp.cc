// linterp.cc: linear spline

#include <cassert>

#include "linterp.h"

namespace pp {

// Function that returns the location of the index i of the interval x[i] <= z < x[i+1] given a vector x
int binsearch(const vector& x, double z)
{
    assert(z >= x[0] && z <= x[x.size()-1]);

    int i = 0;
    int j = x.size() - 1;

    while(j - i > 1){
        int mid = (i + j) / 2;

        if(z > mid)
            i = mid; 
        else
            j = mid;
    }

    return i;
} // binsearch - TASK A.1 (from instructions)


// Function that returns the linear spline interpolation from a table {x[i], y[i]} at a point z
double linterp(const vector& x, const vector& y, double z)
{
    int i = binsearch(x, z);

    double dx = x[i+1] - x[i];
    assert(dx > 0.0);

    double dy = y[i+1] - y[i];

    return y[i] + dy/dx * (z - x[i]);
} // linterp - TASK A.1 (from instructions)


// Function that calculates, analytically, the integral of the linear spline from x[0] to a point z
double linterpInteg(const vector& x, const vector& y, double z)
{
    int i = binsearch(x, z);

    double sum = 0.0;

    for(int k = 0; k < i; k++){
        double dx = x[k+1] - x[k];
        sum += (y[k] + y[k+1]) * dx / 2.0;
    }

    double yz = linterp(x, y, z);
    double dz = z - x[i];

    sum += (y[i] + yz) * dz / 2.0;

    return sum;
} // linterpInteg - TASK A.2

} // namespace pp
