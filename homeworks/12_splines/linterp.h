// linterp.h

#pragma once

#include "vector.h"

namespace pp {

int binsearch(const vector& x, double z);

double linterp(const vector& x, const vector& y, double z);

double linterpInteg(const vector& x, const vector& y, double z);

}
