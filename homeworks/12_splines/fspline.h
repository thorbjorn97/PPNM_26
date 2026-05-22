// fspline.h

#pragma once

#include <functional>

#include "vector.h"

namespace pp {

std::function<double(double)>
make_qspline(vector x, vector y);

} // namespace pp
