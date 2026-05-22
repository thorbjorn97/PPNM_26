// cspline.h

#pragma once

#include "vector.h"

namespace pp {

struct cspline {

    int n;
    vector x, y;

    vector b, c, d;

    cspline(const vector& x, const vector& y);

    double eval(double z) const;
    double deriv(double z) const;
    double integ(double z) const;

private:
    int binsearch(double z) const;
};

} // namespace pp
