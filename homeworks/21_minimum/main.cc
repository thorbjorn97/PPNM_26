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

    // ========================================
    // B.1 - PREPARATION
    // ========================================
    substep("B.1 - PREPARATION");

    // Read formatted data table line-by-line from standard input
    std::vector<double> energy, signal, error;
    std::string line;
    double energy_val, signal_val, error_val;

    while (std::getline(std::cin, line)) {
        // Skip empty lines or comment lines starting with '#'
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::stringstream ss(line);
        if (ss >> energy_val >> signal_val >> error_val) {
            energy.push_back(energy_val);
            signal.push_back(signal_val);
            error.push_back(error_val);
        }
    }
    std::cout << "Successfully read " << energy.size() << " data points in 'higgs.data.txt' from the standard input\n";

    // Define the Breit-Wigner fitting deviation function
    // v[0] = mass (m), v[1] = width (Gamma), v[2] = scale factor (A)
    auto higgs_deviation = [&](const vector& v) {
        double m     = v[0];
        double gamma = v[1];
        double A     = v[2];
        
        double sum_sq_dev = 0.0;
        for (size_t i = 0; i < energy.size(); ++i) {
            double E = energy[i];
            // Breit-Wigner function F(E | m, gamma, A)
            double F = A / (std::pow(E - m, 2) + std::pow(gamma, 2) / 4.0);
            
            double deviation = (F - signal[i]) / error[i];
            sum_sq_dev += deviation * deviation;
        }
        return sum_sq_dev;
    };
    std::cout << "Successfully defined the deviation function: D(m,Γ,A)=Σi[(F(Ei|m,Γ,A)-σi)/Δσi]^2\n";

    // ========================================
    // B.2 - FIT OF BREIT-WIGNER TO HIGGS DATA
    // ========================================
    substep("B.2 - FIT OF BREIT-WIGNER TO HIGGS DATA");
    
    // Guesses based on instruction clues: mass ~ 125, Gamma ~ 2, Scale A ~ 10
    vector start_higgs({125.0, 2.0, 10.0}); 
    
    std::cout << "Initial Guess Parameters:\n"
              << "  Mass (m):     " << start_higgs[0] << " GeV/c^2\n"
              << "  Width (G):    " << start_higgs[1] << " GeV\n"
              << "  Scale (A):    " << start_higgs[2] << "\n\n";

    // Minimize deviation using Newton's method from Task A
    minimizer_result res_higgs = minimize_newton(higgs_deviation, start_higgs, 1e-4, 1000);

    double opt_m     = res_higgs.min_point[0];
    double opt_gamma = std::abs(res_higgs.min_point[1]); // width is physically symmetric/positive
    double opt_A     = res_higgs.min_point[2];

    std::cout << "Optimized Parameters Found:\n"
              << "  Mass (m):     " << opt_m << " GeV/c^2\n"
              << "  Width (G):    " << opt_gamma << " GeV\n"
              << "  Scale (A):    " << opt_A << "\n";
    std::cout << "Final Chi-squared Deviation: " << higgs_deviation(res_higgs.min_point) << "\n";
    std::cout << "Steps Taken to Converge:     " << res_higgs.steps_taken << "\n\n";

    // Save the fitted curve to a file for gnuplot
    std::ofstream fit_file("higgs_fit.dat");
    if (fit_file.is_open()) {
        for (double E = 100.0; E <= 160.0; E += 0.05) {
            double F = opt_A / (std::pow(E - opt_m, 2) + std::pow(opt_gamma, 2) / 4.0);
            fit_file << E << " " << F << "\n";
        }
        fit_file.close();
    }
    std::cout << "Datafile generated: 'higgs_fit.dat'" << "\n";    

    // Save the raw experimental data to an explicit data file for plotting
    std::ofstream exp_file("higgs_data_clean.dat");
    if (exp_file.is_open()) {
        for (size_t i = 0; i < energy.size(); ++i) {
            exp_file << energy[i] << " " << signal[i] << " " << error[i] << "\n";
        }
        exp_file.close();
    }
    std::cout << "Datafile generated: 'higgs_data_clean.dat'" << "\n";    

    std::cout << "Plot: See 'plot.gpi' and 'higgs_fit.svg'" << "\n";

    // ============================================================================================
    // Task C (1 point): Central instead of forward finite difference approximation for the derivatives
    // ============================================================================================
    section("Task C (1 point): Central instead of forward finite difference approximation for the derivatives");
    
    std::cout << "Pending" << "\n\n\n";


} // main
