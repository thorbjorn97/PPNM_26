// ode.h [from homework 15_ode]
// Declarations of Runge–Kutta ODE routines

#pragma once

#include <functional>
#include <tuple>
#include <vector>
#include <limits>

#include "vector.h"

namespace pp {

std::tuple<vector, vector>
rkstep45(std::function<vector(double, vector)> F,
         double x,
         vector y,
         double h);

std::tuple<std::vector<double>, std::vector<vector>>
driver(std::function<vector(double, vector)> F,
       double a,
       double b,
       vector yinit,
       double h,
       double acc,
       double eps);
} // namespace pp