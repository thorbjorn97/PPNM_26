// qspline.h

#pragma once

#include "vector.h"

namespace pp {

struct qspline {

    int n;
    vector x, y;
    vector b, c;

    qspline(const vector& x, const vector& y);

    int binsearch(double z) const;

    double eval(double z) const;
    double deriv(double z) const;
    double integ(double z) const;
};

} // namespace pp
