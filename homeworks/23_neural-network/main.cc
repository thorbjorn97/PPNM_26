// main.cc

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "utils.h"
#include "vector.h"
#include "matrix.h"
#include "cc_integrator.h"  // for Clenshaw-Curtis integrator

#include "ann.h"
#include "target_function.h"

using namespace pp;

int main(){
    // ============================================================================================
    // Task A (6 points)
    // ============================================================================================
    section("Task A (6 points)");

    // ========================================
    // A.1 - IMPLEMENTATION OF ANN
    // ========================================
    substep("A.1 - IMPLEMENTATION OF ANN");

    std::cout << "See 'ann.cc'\n\n";
    std::cout << "Minimizer used: Newton's method with numerical gradient, numerical Hessian matrix and back-tracking linesearch as implemented in 'minimizer.cc' [From homework 21_minimum]\n\n";

    // ========================================
    // A.2 - GENERATION OF TABULATED DATA
    // ========================================
    substep("A.2 - TABULATED FUNCTION DATA");

    // Initialize tabulated function {xk,yk}_{k=1..N}
    const size_t N = 35;    // number of training data points (moderate to prevent overfitting)
    vector x_train(N);      // x-values
    vector y_train(N);      // y-values

    std::cout << "Target function selected: g(x) = cos(5x-1) * exp(-x²)\n\n";
    
    // Generate training data
    std::ofstream training_file("training_points.dat");
    if (!training_file.is_open()) {
        std::cerr << "Error: Could not create training_points.dat\n";
        return 1;
    }
    for (size_t i = 0; i < N; ++i) {
        double x = -1.0 + 2.0 * double(i) / double(N - 1); // define x-values on [-1, 1] (as per instructions), here equidistantly
        x_train[i] = x;     // store x
        y_train[i] = g(x);  // compute y from exact function
        
        training_file << x_train[i] << " " << y_train[i] << "\n";
    }
    training_file.close();
    std::cout << "Successfully generated N=" << N << " training data points {xk,yk}_{k=1..N} from g(x) uniformly on x=[-1, 1]\n\n";
    std::cout << "Datafile created: 'training_points.dat'\n\n";

    // ========================================
    // A.3 - TRAINING OF ANN
    // ========================================
    substep("A.3 - TRAINING OF ANN");

    // Initialize network
    int hidden_neurons = 5;  // number of hidden neurons (balance between overfitting and underfitting)
    ann network(hidden_neurons);  // initialize network
    std::cout << "Created ANN with " << hidden_neurons << " hidden neurons\n\n";

    // Train network
    std::cout << "Starting network training to approximate g(x) on given training data points...\n";
    network.train(x_train, y_train);  // train network on tabulated data
    std::cout << "Training complete\n\n";
    std::cout << "Minimizer steps taken: " << network.steps << "\n\n";

    // ========================================
    // A.4 - EVALUATION OF ANN VS EXACT FUNCTION
    // ========================================
    substep("A.4 - EVALUATION OF ANN VS EXACT FUNCTION");

    const size_t n_samples = 300;  // number of samples

    std::cout << "Evaluating network on grid with " << n_samples << " samples...\n";
    // Evaluate network on high-resolution grid
    std::ofstream output_file("ann_interpolation.dat");
    if (!output_file.is_open()) {
        std::cerr << "Error: Could not create ann_interpolation.dat\n";
        return 1;
    }
    output_file << "# X   Exact_g(X)   ANN_Response(X)\n";
    for (size_t j = 0; j < n_samples; ++j) {                        // for sample j
        double x_j = -1.0 + 2.0 * double(j) / double(n_samples - 1);  // map j equidistantly to [-1, 1] to define x-value x_j
        double exact_y_j = g(x_j);                                    // evaluate exact function at x_j
        double network_y_j = network.response(x_j);                   // evaluate network response at input signal x_j

        output_file << x_j << " " << exact_y_j << " " << network_y_j << "\n";
    }
    output_file.close();
    std::cout << "Evaluation complete\n\n";
    std::cout << "Datafile created: 'ann_interpolation.dat'\n\n";

    // ========================================
    // A.5 - PLOT
    // ========================================
    substep("A.5 - PLOT");

    std::cout << "See 'plot.gpi' and 'ann_results.svg'\n";

    // ============================================================================================
    // Task B (3 points)
    // ============================================================================================
    section("Task B (3 points)");
    
    // ========================================
    // B.1 - MODIFICATION OF ANN
    // ========================================
    substep("B.1 - MODIFICATION OF ANN");

    std::cout << "See 'ann.cc'\n\n";

    // ========================================
    // B.2 - GENERATION OF TABULATED DATA
    // ========================================
    substep("B.2 - TABULATED FUNCTION DATA");

    std::ofstream calculus_file("ann_calculus.dat");
    if (!calculus_file.is_open()) {
        std::cerr << "Error: Could not create ann_calculus.dat\n";
        return 1;
    }
    double ann_integral_origin = network.response_integral(-1.0);  // compute baseline shift at the lower bound x=-1
    calculus_file << "# X   ANN_y   Exact_y   ANN_dy   Exact_dy   ANN_d2y   Exact_d2y   ANN_Integral   Exact_Integral\n";
    for (size_t j = 0; j < n_samples; ++j) {                        // for sample j
        double x_j = -1.0 + 2.0 * double(j) / double(n_samples - 1);  // map j equidistantly to [-1, 1] to define x-value x_j

        // Evaluate new analytical calculus pipelines
        double ann_y   = network.response(x_j);
        double ann_dy  = network.response_derivative(x_j);
        double ann_d2y = network.response_second_derivative(x_j);
        double ann_int = network.response_integral(x_j) - ann_integral_origin;  // subtract the baseline shift to eliminate vertical offset

        // Evaluate target math functions exactly
        double exact_y   = g(x_j);
        double exact_dy  = g_derivative(x_j);
        double exact_d2y = g_second_derivative(x_j);
        double num_exact_int = integrate_cc(g, -1.0, x_j, 1e-7, 1e-7).value;  // numerical integration of g(x) on [-1, x_j]

        // Stream all properties to the data file
        calculus_file << x_j       << " " 
                      << ann_y     << " " << exact_y   << " " 
                      << ann_dy    << " " << exact_dy  << " " 
                      << ann_d2y   << " " << exact_d2y << " " 
                      << ann_int   << " " << num_exact_int <<"\n";
    }
    calculus_file.close();
    std::cout << "Datafile created: 'ann_calculus.dat'\n\n";
    
    // ========================================
    // B.3 - PLOT
    // ========================================
    substep("B.3 - PLOT");

    std::cout << "See 'plot.gpi' and 'ann_results.svg'\n";

    // ============================================================================================
    // Task C (1 point)
    // ============================================================================================
    section("Task C (1 point)");
    
    std::cout << "Pending\n\n\n";

} // main
