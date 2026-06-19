// test_functions.h

#pragma once

#include "vector.h"

namespace pp {

double unit_circle(const vector& x);

double gaussian(const vector& x);

double ellipsoid(const vector& x);

double smooth_test(const vector& x);

double singular_integrand(const vector& x);

} // namespace pp
