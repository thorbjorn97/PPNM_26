// main.cc

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "utils.h"
#include "vector.h"
#include "matrix.h"

#include "minimizer.h"
#include "qr.h"  // for QR decomposition (from homework 09_lineq)

using namespace pp;

int main(){
    // ============================================================================================
    // Task A (6 points): Newton's method with numerical gradient, numerical Hessian matrix and back-tracking linesearch
    // ============================================================================================
    section("Task A (6 points): Newton's method with numerical gradient, numerical Hessian matrix and back-tracking linesearch");

    // ========================================
    // A.1 - IMPLEMENTATION
    // ========================================
    substep("A.1 - IMPLEMENTATION");

    std::cout << "See 'minimizer.cc'\n\n";

    // ========================================
    // A.2 - TESTS
    // ========================================
    substep("A.2 - TESTS");

    // 1. Rosenbrock's Valley Function
    // Global minimum is located at (1, 1) where f(x, y) = 0
    auto rosenbrock = [](const vector& v) {
        double x = v[0];
        double y = v[1];
        return std::pow(1.0 - x, 2) + 100.0 * std::pow(y - x * x, 2);
    };

    substep("Minimizing Rosenbrock's Valley Function");
    vector start_rosenbrock({-1.2, 1.0}); // Standard test starting coordinates
    std::cout << "Starting Position:\n";
    print_vector(start_rosenbrock);

    minimizer_result res_rosenbrock = minimize_newton(rosenbrock, start_rosenbrock, 1e-3, 1000, "rosenbrock_path.dat");    
    
    std::cout << "\nFound Minimum Position:\n";
    print_vector(res_rosenbrock.min_point);
    std::cout << "Function Value at Minimum: " << rosenbrock(res_rosenbrock.min_point) << "\n";
    std::cout << "Steps Taken:               " << res_rosenbrock.steps_taken << "\n";

    // 2. Himmelblau's Function
    // Has 4 identical global minima where f(x, y) = 0
    // Has a local maximum at (-0.270845, -0.923039)
    auto himmelblau = [](const vector& v) {
        double x = v[0];
        double y = v[1];
        return std::pow(x * x + y - 11.0, 2) + std::pow(x + y * y - 7.0, 2);
    };

    substep("Minimizing Himmelblau's Function");
    vector start_himmelblau({-0.27, -0.92}); // Chosen near local maximum to stress test the linesearch
    std::cout << "Starting Position:\n";
    print_vector(start_himmelblau);

    minimizer_result res_himmelblau = minimize_newton(himmelblau, start_himmelblau, 1e-3, 1000, "himmelblau_path.dat");

    std::cout << "\nFound Minimum Position:\n";
    print_vector(res_himmelblau.min_point);
    std::cout << "Function Value at Minimum: " << himmelblau(res_himmelblau.min_point) << "\n";
    std::cout << "Steps Taken:               " << res_himmelblau.steps_taken << "\n\n";

    // ========================================
    // A.3 - (NON-MANDATORY) PLOTS
    // ========================================
    substep("A.3 - (NON-MANDATORY) PLOTS");

    std::cout << "See 'plot.gpi', 'rosenbrock_convergence.svg' and 'himmelblau_convergence.svg'\n";

    // ============================================================================================
    // Task B (3 points): Higgs boson discovery
    // ============================================================================================
    section("Task B (3 points): Higgs boson discovery");
    
    std::cout << "Pending" << "\n";

    // ============================================================================================
    // Task C (1 point): Central instead of forward finite difference approximation for the derivatives
    // ============================================================================================
    section("Task C (1 point): Central instead of forward finite difference approximation for the derivatives");
    
    std::cout << "Pending" << "\n\n\n";


} // main
