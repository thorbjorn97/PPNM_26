// integrator.cc
// Implementation of the recursive open 4-point adaptive integrator (Task A) and error estimation (Task C)

#include <iostream>
#include <cmath>

#include "integrator.h"
#include "cc_integrator.h"

static int ncalls = 0; // Global variable to count integrand evaluations

// Helper function to evaluate the integrand and count integrand evaluations
static double feval(
    const std::function<double(double)>& f,
    double x
){
    ++ncalls;     // Update count of integrand evaluations
    return f(x);  // Evaluate the integrand at the given point
}

// Recursive adaptive integrator function
static IntegrationResult adapt(
    const std::function<double(double)>& f,
    double a,   // Lower bound
    double b,   // Upper bound
    double acc, // Absolute accuracy goal
    double eps, // Relative accuracy goal
    double f2,  // f(a + 2*h/6)
    double f3,  // f(a + 4*h/6)
    bool reuse  // To reuse of points f2 and f3
){
    double h = b - a;  // Interval length

    // CRITICAL: Stop infinite recursion near singularities
    if (std::abs(h) < 1e-15) {
        return {0.0, 0.0};
    }

    if(!reuse){ // First call, no points to reuse
        f2 = feval(f, a + 2*h/6);
        f3 = feval(f, a + 4*h/6);
    }

    // Adaptive quadrature points
    double f1 = feval(f, a + h/6);
    double f4 = feval(f, a + 5*h/6);

    // High-order Q and low-order q estimate
    double Q = (2*f1 + f2 + f3 + 2*f4) / 6.0 * h; // Higher order rule
    double q = (f1 + f2 + f3 + f4) / 4.0 * h;     // Lower order rule

    // Error estimate and tolerance
    double err = std::abs(Q - q);
    double tol = acc + eps * std::abs(Q);

    // Base case: If error is within tolerance (accuracy goal reached), return evaluated area (Task A) and error (Task C)
    if(err < tol){
        return {Q, err};
    }

    // Recursive case: Subdivide integration interval and apply adapt to each half
    double mid = (a + b) / 2.0;

    auto left = adapt(
        f, a, mid,
        acc / std::sqrt(2.0),
        eps,
        f1, f2,
        true
    );

    auto right = adapt(
        f, mid, b,
        acc / std::sqrt(2.0),
        eps,
        f3, f4,
        true
    );

    return {
        left.value + right.value,          // Final value (Task A)
        std::sqrt(left.error*left.error +  
                  right.error*right.error) // Error estimate (Task C)
    };
}

// Public interface to the adaptive integrator, resets count and initiates recursion
IntegrationResult integrate(
    const std::function<double(double)>& f,
    double a,
    double b,
    double acc,
    double eps
){
    ncalls = 0; // Reset count
    return adapt( // Forward to recursive adapt function with initial parameters
        f, a, b,
        acc, eps,
        0.0, 0.0,
        false
    );
}

int get_integration_calls(){
    return ncalls;
}
