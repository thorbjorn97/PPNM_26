// broyden.h
// Based on code from minimizer.h in homework 21_minimum
// Additions to this homework are marked with {new}

#pragma once

#include <functional>
#include <string>

#include "vector.h"
#include "matrix.h"

namespace pp {

// {new} Global configuration 
extern std::string broyden_log_filename;
extern int broyden_steps;
extern double broyden_final_z;
extern double broyden_final_gnorm;
extern std::string broyden_exit_reason;

struct minimizer_result {
    vector min_point;
    int steps_taken;
};

// Numerical gradient calculation with baseline evaluation caching
vector gradient(
    const std::function<double(const vector&)>& phi,
    const vector& x,
    double phi_x
);
// ^Optimized: Receives cached phi(x) to save 1 function evaluation per call

// Numerical Hessian calculation using finite differences of the gradient
matrix hessian(
    const std::function<double(const vector&)>& phi,
    const vector& x,
    const vector& g_x
);
// ^Optimized: Receives the already-computed gradient vector to save (1 + n) evaluations

// // Newton's minimization method with numerical gradient, numerical Hessian and backtracking line search
// minimizer_result minimize_newton(
//     const std::function<double(const vector&)>& phi,
//     const vector& x_start,
//     double acc = 1e-3,
//     int max_iter = 1000,
//     const std::string& log_filename = "" // argument for logging
// );

// {new} Quasi-Newton minimization with Broyden's update (Standard Broyden), matching the exact requested signature
vector broyden(
    std::function<double(const vector&)> f,
    vector x,
    double acc
);

// {new} Quasi-Newton minimization with Symmetrized Broyden's update (BFGS), for comparison
vector broyden_symmetrized(
    std::function<double(const vector&)> f,
    vector x,
    double acc
);

} // namespace pp
