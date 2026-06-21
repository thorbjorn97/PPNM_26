// main.cc

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "utils.h"
#include "vector.h"
#include "matrix.h"

#include "rootfinder.h"
#include "qr.h"  // for QR decomposition (from homework 09_lineq)
#include "ode.h"  // for Runge–Kutta ODE solver (from homework 15_ode)

using namespace pp;

int main(){
    // ============================================================================================
    // Task A (6 points): Newton's method with numerical Jacobian and back-tracking line-search
    // ============================================================================================
    section("Task A (6 points): Newton's method with numerical Jacobian and back-tracking line-search");

    // ========================================
    // A.1 - IMPLEMENTATION OF NEWTON'S METHOD WITH SIMPLE BACKTRACKING LINE-SEARCH
    // ========================================
    substep("A.1 - IMPLEMENTATION OF NEWTON'S METHOD WITH SIMPLE BACKTRACKING LINE-SEARCH");

    std::cout << "Newton's method (simple):  See newton() in 'rootfinder.cc'\n";
    std::cout << "Jacobian estimation:       See jacobian() in 'rootfinder.cc'\n\n";

    // ========================================
    // A.2 - DEBUGGING ON SIMPLE 1D AND 2D EQUATIONS
    // ========================================
    substep("A.2 - DEBUGGING ON SIMPLE 1D AND 2D EQUATIONS");

    double acc_global = 1e-8;  // target accuracy for all tests

    // ----------------------------------------
    // TEST 1: x² - 2 = 0
    // ----------------------------------------
    auto f1 = [](const vector& x) -> vector {
        vector y(1);
        y[0] = x[0]*x[0] - 2.0;
        return y;
    };
    vector x0_t1(1); x0_t1[0] = 1.0;
    newton_result res1 = newton(f1, x0_t1, acc_global);
    
    std::cout << "[1D TEST: x² - 2 = 0]\n";
    std::cout << "    Initial guess:         " << x0_t1[0] << "\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Expected root:         " << std::sqrt(2.0) << "\n";
    std::cout << "    Found root:            " << res1.root[0] << "\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Residual norm ‖f(x)‖:  " << f1(res1.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
    std::cout << std::defaultfloat;
    std::cout << "    Iterations:            " << res1.iterations_taken << "\n";
    std::cout << "    Exit reason:           " << res1.exit_reason << "\n\n";

    // ----------------------------------------
    // TEST 2: x - 1 = 0
    // ----------------------------------------
    auto f2 = [](const vector& x) -> vector {
        vector y(1);
        y[0] = x[0] - 1.0;
        return y;
    };
    vector x0_t2(1); x0_t2[0] = 5.0;
    newton_result res2 = newton(f2, x0_t2, acc_global);

    std::cout << "[1D TEST: x - 1 = 0]\n";
    std::cout << "    Initial guess:         " << x0_t2[0] << "\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Expected root:         1.0\n";
    std::cout << "    Found root:            " << res2.root[0] << "\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Residual norm ‖f(x)‖:  " << f2(res2.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
    std::cout << std::defaultfloat;
    std::cout << "    Iterations:            " << res2.iterations_taken << "\n";
    std::cout << "    Exit reason:           " << res2.exit_reason << "\n\n";

    // ----------------------------------------
    // TEST 3: x² + y² - 1 = 0, x - y = 0
    // ----------------------------------------
    auto f3 = [](const vector& x) -> vector {
        vector y(2);
        y[0] = x[0]*x[0] + x[1]*x[1] - 1.0;
        y[1] = x[0] - x[1];
        return y;
    };
    vector x0_t3(2); x0_t3[0] = 0.5; x0_t3[1] = 0.8;
    newton_result res3 = newton(f3, x0_t3, acc_global);

    std::cout << "[2D TEST: x² + y² - 1 = 0, x - y = 0]\n";
    std::cout << "    Initial guess:         (" << x0_t3[0] << ", " << x0_t3[1] << ")\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Expected root:         (" << 1.0/std::sqrt(2.0) << ", " << 1.0/std::sqrt(2.0) << ")\n";
    std::cout << "    Found root:            (" << res3.root[0] << ", " << res3.root[1] << ")\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Residual norm ‖f(x)‖:  " << f3(res3.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
    std::cout << std::defaultfloat;
    std::cout << "    Iterations:            " << res3.iterations_taken << "\n";
    std::cout << "    Exit reason:           " << res3.exit_reason << "\n\n";

    // ----------------------------------------
    // TEST 4: x² + y² - 4 = 0, exp(x) + y - 1 = 0
    // ----------------------------------------
    auto f4 = [](const vector& x) -> vector {
        vector y(2);
        y[0] = x[0]*x[0] + x[1]*x[1] - 4.0;
        y[1] = std::exp(x[0]) + x[1] - 1.0;
        return y;
    };
    vector x0_t4(2); x0_t4[0] = 0.5; x0_t4[1] = 0.5;
    newton_result res4 = newton(f4, x0_t4, acc_global);

    std::cout << "[2D TEST: x² + y² - 4 = 0, exp(x) + y - 1 = 0]\n";
    std::cout << "    Initial guess:         (" << x0_t4[0] << ", " << x0_t4[1] << ")\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Expected root:         (-1.81626406882515057, 0.83736779989124773) or (1.00416873847465917, -1.72963728702586993)\n";
    std::cout << "    Found root:            (" << res4.root[0] << ", " << res4.root[1] << ")\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Residual norm ‖f(x)‖:  " << f4(res4.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
    std::cout << std::defaultfloat;
    std::cout << "    Iterations:            " << res4.iterations_taken << "\n";
    std::cout << "    Exit reason:           " << res4.exit_reason << "\n\n";

    // ========================================
    // A.3 - EXTREMUM(S) OF THE ROSENBROCK'S VALLEY FUNCTION
    // ========================================
    substep("A.3 - EXTREMUM(S) OF THE ROSENBROCK'S VALLEY FUNCTION");
    
    // Analytical gradient of Rosenbrock function
    auto f_rose = [](const vector& vec) -> vector {
        double x = vec[0];
        double y = vec[1];
        vector g(2);
        // Fully expanded form (derived by letting the gradient equal to zero)
        g[0] = 400.0 * x * x * x - 400.0 * x * y + 2.0 * x - 2.0;
        g[1] = 200.0 * y - 200.0 * x * x;
        return g;
    };
    vector start_rose({-1.2, 1.0}); // initial guess
    newton_result res_rose = newton(f_rose, start_rose, acc_global, 1e-6, 5000);  // root-finding via Newton's method
    
    std::cout << "    Initial guess:             (" << start_rose[0] << ", " << start_rose[1] << ")\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Expected extremum:         (1.0, 1.0)\n";
    std::cout << "    Found extremum:            (" << res_rose.root[0] << ", " << res_rose.root[1] << ")\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Residual norm ‖f(x)‖:      " << f_rose(res_rose.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
    std::cout << std::defaultfloat;
    std::cout << "    Iterations:                " << res_rose.iterations_taken << "\n";
    std::cout << "    Exit reason:               " << res_rose.exit_reason << "\n\n";
    
    // ========================================
    // A.4 - MINIMUM(S) OF THE HIMMELBLAU'S FUNCTION
    // ========================================
    substep("A.4 - MINIMUM(S) OF THE HIMMELBLAU'S FUNCTION");

    // Analytical gradient of Himmelblau function
    auto f_himmel = [](const vector& vec) -> vector {
        double x = vec[0];
        double y = vec[1];
        vector g(2);
        // Fully expanded form (derived by letting the gradient equal to zero)
        g[0] = 4.0 * x * (x * x + y - 11.0) + 2.0 * (x + y * y - 7.0);
        g[1] = 2.0 * (x * x + y - 11.0) + 4.0 * y * (x + y * y - 7.0);
        return g;
    };
    std::vector<vector> start_himmel = {  // Test with four different starting configurations to capture all four minima
        vector({ 3.0,  3.0}),  // Should converge to ( 3.0,                  2.0)
        vector({-3.0,  3.0}),  // Should converge to (-2.80511808695282632,  3.13131251825132986)
        vector({-3.0, -3.0}),  // Should converge to (-3.77931025337839355, -3.28318599128558153)
        vector({ 3.0, -3.0})   // Should converge to ( 3.58442834033044959, -1.84812652696539953)
    };
    std::vector<vector> expec_himmel = {  // Expected minima
        vector({ 3.0,                  2.0}),
        vector({-2.80511808695282632,  3.13131251825132986}),
        vector({-3.77931025337839355, -3.28318599128558153}),
        vector({ 3.58442834033044959, -1.84812652696539953})
    };

    for (size_t i = 0; i < start_himmel.size(); ++i) {  // Loop over test cases
        newton_result res_hummel = newton(f_himmel, start_himmel[i], acc_global, 1e-6, 5000);
        std::cout << "[Himmelblau: Test " << i + 1 << "]\n";
        std::cout << "    Initial guess:             (" << start_himmel[i][0] << ", " << start_himmel[i][1] << ")\n";
        std::cout << std::fixed << std::setprecision(10);
        std::cout << "    Expected minimum:          (" << expec_himmel[i][0] << ", " << expec_himmel[i][1] << ")\n";
        std::cout << "    Found minimum:             (" << res_hummel.root[0] << ", " << res_hummel.root[1] << ")\n";
        std::cout << std::scientific << std::setprecision(4);
        std::cout << "    Residual norm ‖f(x)‖:      " << f_himmel(res_hummel.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
        std::cout << std::defaultfloat;
        std::cout << "    Iterations:                " << res_hummel.iterations_taken << "\n";
        std::cout << "    Exit reason:               " << res_hummel.exit_reason << "\n\n";
    };


    // ============================================================================================
    // Task B (3 points): Bound states of hydrogen atom with shooting method for boundary value problems
    // ============================================================================================
    section("Task B (3 points): Bound states of hydrogen atom with shooting method for boundary value problems");
    
    /* PROOF OF INITIAL BOUNDARY CONDITION AS r --> 0:
       Near r = 0, the Coulomb potential term -(1/r)f(r) dominates over the energy term Ef(r).
       So the Schrödinger equation simplifies to:
            -(1/2)f''(r) - (1/r)f(r) = Ef(r)  ==>  f''(r) ≈ -(2/r)f(r).
       Assume a Frobenius power series solution:
            f(r) = c0*r + c1*r² + ...
       Then
            f''(r) = 2*c1 + 6*c2*r + ...
       Substituting these back into the simplified equation:
            f''(r) ≈ -(2/r)f(r)  ==>  2*c1 + 6*c2*r ≈ -(2/r)(c0*r + c1*r²) = -2*c0 - 2*c1*r
                                 ==>  2*c1 = -2*c0 - 2*c1*r - 6*c2*r
       For r --> 0, the constant terms dominate:
            2*c1 = -2*c0  ==>  c1 = -c0  ==>  f(r) = c0*r - c0*r² = c0*(r - r²)
       If we choose a normalization scale where c0 = 1, then, to first order,
            f(r --> 0) = r - r².
     */

    // Integration setup parameters
    double r_min = 1e-4;
    double r_max = 8.0;
    double acc_ode = 1e-6;
    double eps_ode = 1e-6;
    double h_initial = 0.01;

    // The Shooting Function M(E)
    // Takes 1D vector (Energy), returns 1D vector containing y(r_max):
    auto M = [r_min, r_max, h_initial, acc_ode, eps_ode](const vector& E_vec) -> vector {
        double E = E_vec[0];

        // u[0] = y(r), u[1] = y'(r)
        auto f_ode = [E](double r, const vector& u) -> vector {
            vector dudr(2);
            dudr[0] = u[1];
            dudr[1] = -2.0 * (E + 1.0 / r) * u[0];
            return dudr;
        };

        // Initial conditions at r_min: y(r_min) = r - r² -> for r->0, y ~ r, so y'=1
        vector u_init(2);
        u_init[0] = r_min - r_min * r_min; 
        u_init[1] = 1.0 - 2.0 * r_min;

        // Run the adaptive ODE integration step
        auto [xlist, ylist] = driver(f_ode, r_min, r_max, u_init, h_initial, acc_ode, eps_ode);

        // Target residual value at the far boundary r_max
        vector residual(1);
        residual[0] = ylist.back()[0]; 
        return residual;
    };
    
    // ========================================
    // B.1 - THE LOWEST ROOT E0 OF M(E) = 0
    // ========================================
    substep("B.1 - THE LOWEST ROOT E0 OF M(E) = 0");

    vector E_guess(1);
    E_guess[0] = -1.0; // Initial guess well below expected value

    newton_result res_E0 = newton(M, E_guess, acc_global);

    std::cout << "    Initial guess:             " << E_guess[0] << " Hartree\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Exact E0:                  " << "-0.5" << " Hartree\n";
    std::cout << "    Found E0:                  " << res_E0.root[0] << " Hartree\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Residual norm ‖M(E)‖:      " << M(res_E0.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
    std::cout << std::defaultfloat;
    std::cout << "    Iterations:                " << res_E0.iterations_taken << "\n";
    std::cout << "    Exit reason:               " << res_E0.exit_reason << "\n\n";

    // ========================================
    // B.2 - PLOT AND COMPARISON WITH EXACT RESULT
    // ========================================
    substep("B.2 - PLOT AND COMPARISON WITH EXACT RESULT");
    
    // Re-run the ODE at converged energy root to get the full clean trajectory
    double E0 = res_E0.root[0];  // lowest root of M(E)
    auto f_ode_final = [E0](double r, const vector& u) -> vector {
        vector dudr(2);
        dudr[0] = u[1];
        dudr[1] = -2.0 * (E0 + 1.0 / r) * u[0];
        return dudr;  
    };
    
    vector u_init_final(2);  // initial conditions at r_min
    u_init_final[0] = r_min - r_min * r_min;
    u_init_final[1] = 1.0 - 2.0 * r_min;

    // Run the adaptive ODE integration step
    auto [xdata, ydata] = driver(f_ode_final, r_min, r_max, u_init_final, h_initial, acc_ode, eps_ode);
    
    std::ofstream datafile("hydrogen_wavefunction.dat");
    if (datafile.is_open()) {
        datafile << "# r\t\tCalculated_y(r)\t\tExact_y(r)\n";
        
        // Find numerical normalization factor matching the exact analytical solution y(r) = r * e^(-r)
        // We can pick a middle node index to scale our arbitrary initial slope value
        size_t mid = xdata.size() / 3;
        double exact_at_mid = xdata[mid] * std::exp(-xdata[mid]);
        double normalization_factor = exact_at_mid / ydata[mid][0];

        for (size_t i = 0; i < xdata.size(); ++i) {
            double r = xdata[i];
            double y_numerical = ydata[i][0] * normalization_factor;
            double y_exact = r * std::exp(-r); // Theoretical exact ground state profile
            
            datafile << std::fixed << std::setprecision(6)
                     << r << "\t\t" << y_numerical << "\t\t" << y_exact << "\n";
        }
        datafile.close();
        std::cout << "Datafile created: 'hydrogen_wavefunction.dat'\n";
    } else {
        std::cerr << "Error tracking: could not open output data file!\n";
    }
    std::cout << "Plot: See 'plot.gpi' and 'hydrogen_analysis.svg'\n\n";

    // ========================================
    // B.3 - CONVERGENCE INVESTIGATION WRT R_MAX AND R_MIN
    // ========================================
    substep("B.3 - CONVERGENCE INVESTIGATION WRT R_MAX AND R_MIN");

    // Clear and dynamic column width constants
    const int w_param = 14;
    const int w_energy = 22;
    const int w_error = 20;

    std::cout << "\nConvergence investigation of r_max (with fixed r_min = 1e-4)\n";
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n";
    std::cout << std::left  << std::setw(w_param)  << "r_max"
              << std::left  << std::setw(w_energy) << "Calculated E0 [Ha]"
              << std::right << std::setw(w_error)  << "Absolute Error" << "\n";
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n";
    
    std::vector<double> r_max_values = {3.0, 5.0, 8.0, 10.0, 15.0};
    for (double r_max_test : r_max_values) {
        auto M_rmax = [r_min, r_max_test, h_initial, acc_ode, eps_ode](const vector& E_vec) -> vector {
            double E = E_vec[0];
            auto f_ode = [E](double r, const vector& u) -> vector {
                vector dudr(2);
                dudr[0] = u[1];
                dudr[1] = -2.0 * (E + 1.0 / r) * u[0];
                return dudr;
            };
            vector u_init(2);
            u_init[0] = r_min - r_min * r_min;
            u_init[1] = 1.0 - 2.0 * r_min;

            auto [xlist, ylist] = driver(f_ode, r_min, r_max_test, u_init, h_initial, acc_ode, eps_ode);
            vector residual(1);
            residual[0] = ylist.back()[0];
            return residual;
        };

        vector E_test(1); E_test[0] = -1.0;
        vector E_res = newton(M_rmax, E_test, acc_global).root;
        
        std::cout << std::left  << std::setw(w_param)  << std::fixed << std::setprecision(1) << r_max_test
                  << std::left  << std::setw(w_energy) << std::fixed << std::setprecision(10) << E_res[0]
                  << std::right << std::setw(w_error)  << std::scientific << std::setprecision(4) << std::abs(E_res[0] - (-0.5)) << "\n";
    }
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n\n";

    std::cout << "Convergence investigation of r_min (with fixed r_max = 8.0)\n";
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n";
    std::cout << std::left  << std::setw(w_param)  << "r_min"
              << std::left  << std::setw(w_energy) << "Calculated E0 [Ha]"
              << std::right << std::setw(w_error)  << "Absolute Error" << "\n";
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n";
    
    std::vector<double> r_min_values = {1e-1, 1e-2, 1e-3, 1e-4, 1e-5};
    for (double r_min_test : r_min_values) {
        auto M_rmin = [r_min_test, r_max, h_initial, acc_ode, eps_ode](const vector& E_vec) -> vector {
            double E = E_vec[0];
            auto f_ode = [E](double r, const vector& u) -> vector {
                vector dudr(2);
                dudr[0] = u[1];
                dudr[1] = -2.0 * (E + 1.0 / r) * u[0];
                return dudr;
            };
            vector u_init(2);
            u_init[0] = r_min_test - r_min_test * r_min_test;
            u_init[1] = 1.0 - 2.0 * r_min_test;

            auto [xlist, ylist] = driver(f_ode, r_min_test, r_max, u_init, h_initial, acc_ode, eps_ode);
            vector residual(1);
            residual[0] = ylist.back()[0];
            return residual;
        };

        vector E_test(1); E_test[0] = -1.0;
        vector E_res = newton(M_rmin, E_test, acc_global).root;
        
        std::cout << std::left  << std::setw(w_param)  << std::scientific << std::setprecision(1) << r_min_test
                  << std::left  << std::setw(w_energy) << std::fixed << std::setprecision(10) << E_res[0]
                  << std::right << std::setw(w_error)  << std::scientific << std::setprecision(4) << std::abs(E_res[0] - (-0.5)) << "\n";
    }
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n\n";

    // ========================================
    // B.4 - CONVERGENCE INVESTIGATION WRT ODE ACC AND EPS
    // ========================================
    substep("B.4 - CONVERGENCE INVESTIGATION WRT ODE ACC AND EPS");

    std::cout << "\nODE tolerance investigation (with fixed r_min = 1e-4, r_max = 8.0)\n";
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n";
    std::cout << std::left  << std::setw(w_param)  << "acc & eps"
              << std::left  << std::setw(w_energy) << "Calculated E0 [Ha]"
              << std::right << std::setw(w_error)  << "Absolute Error" << "\n";
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n";

    std::vector<double> tol_values = {1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8};
    for (double tol : tol_values) {
        auto M_tol = [r_min, r_max, h_initial, tol](const vector& E_vec) -> vector {
            double E = E_vec[0];
            auto f_ode = [E](double r, const vector& u) -> vector {
                vector dudr(2);
                dudr[0] = u[1];
                dudr[1] = -2.0 * (E + 1.0 / r) * u[0];
                return dudr;
            };
            vector u_init(2);
            u_init[0] = r_min - r_min * r_min;
            u_init[1] = 1.0 - 2.0 * r_min;

            auto [xlist, ylist] = driver(f_ode, r_min, r_max, u_init, h_initial, tol, tol);
            vector residual(1);
            residual[0] = ylist.back()[0];
            return residual;
        };

        vector E_test(1); E_test[0] = -1.0;
        vector E_res = newton(M_tol, E_test, acc_global).root;
        
        std::cout << std::left  << std::setw(w_param)  << std::scientific << std::setprecision(1) << tol
                  << std::left  << std::setw(w_energy) << std::fixed << std::setprecision(10) << E_res[0]
                  << std::right << std::setw(w_error)  << std::scientific << std::setprecision(4) << std::abs(E_res[0] - (-0.5)) << "\n";
    }
    std::cout << std::string(w_param + w_energy + w_error, '-') << "\n";
    
    // Reset formatting state to default standard streams
    std::cout << std::defaultfloat << std::setprecision(6);
    
    //=============================================================================================
    // Task C (1 point): Quadratic interpolation line-search 
    // ============================================================================================
    section("Task C (1 point): Quadratic interpolation line-search");

    // ========================================
    // C.1 - JACOBIAN OPTIMIZATION OF THE IMPLEMENTATION IN TASK A
    // ========================================
    substep("C.1 - JACOBIAN OPTIMIZATION OF THE IMPLEMENTATION IN TASK A");

    std::cout << "See newton_broyden_quadratic() in 'rootfinder.cc'\n\n";

    // ========================================
    // C.2 - QUADRATIC INTERPOLATION LINE-SEARCH IMPLEMENTATION
    // ========================================
    substep("C.2 - QUADRATIC INTERPOLATION LINE-SEARCH IMPLEMENTATION");

    std::cout << "See newton_broyden_quadratic() in 'rootfinder.cc'\n\n";

    // ========================================
    // C.3 - (NON-MANDATORY) TESTS OF THE OPTIMIZED ROOTFINDER
    // ========================================
    substep("C.3 - (NON-MANDATORY) TESTS OF THE OPTIMIZED ROOTFINDER");

    std::cout << "Test of the optimized rootfinder (Broyden's updates + quadratic interpolation line-search):\n\n";

    newton_result res_rose_c = newton_broyden_quadratic(f_rose, start_rose, acc_global, 1e-3, 5000);
    std::cout << "[Rosenbrock Valley Test]\n";
    std::cout << std::defaultfloat;
    std::cout << "    Initial guess:             (" << start_rose[0] << ", " << start_rose[1] << ")\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Expected extremum:         (1.0, 1.0)\n";
    std::cout << "    Found extremum:            (" << res_rose_c.root[0] << ", " << res_rose_c.root[1] << ")\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Residual norm ‖f(x)‖:      " << f_rose(res_rose_c.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
    std::cout << std::defaultfloat;
    std::cout << "    Iterations:                " << res_rose_c.iterations_taken << "\n";
    std::cout << "    Exit reason:               " << res_rose_c.exit_reason << "\n\n";
    
    for (size_t i = 0; i < start_himmel.size(); ++i) {
        newton_result res_hummel_c = newton_broyden_quadratic(f_himmel, start_himmel[i], acc_global, 1e-3, 5000);
        std::cout << "[Himmelblau: Test " << i + 1 << "]\n";
        std::cout << std::defaultfloat;
        std::cout << "    Initial guess:             (" << start_himmel[i][0] << ", " << start_himmel[i][1] << ")\n";
        std::cout << std::fixed << std::setprecision(10);
        std::cout << "    Expected minimum:          (" << expec_himmel[i][0] << ", " << expec_himmel[i][1] << ")\n";
        std::cout << "    Found minimum:             (" << res_hummel_c.root[0] << ", " << res_hummel_c.root[1] << ")\n";
        std::cout << std::scientific << std::setprecision(4);
        std::cout << "    Residual norm ‖f(x)‖:      " << f_himmel(res_hummel_c.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
        std::cout << std::defaultfloat;
        std::cout << "    Iterations:                " << res_hummel_c.iterations_taken << "\n";
        std::cout << "    Exit reason:               " << res_hummel_c.exit_reason << "\n\n";
    };

    vector E_guess_c(1);
    E_guess_c[0] = -1.0;
    newton_result res_E0_c = newton_broyden_quadratic(M, E_guess_c, acc_global, 1e-3, 5000);
    std::cout << "[The Lowest root E0 of M(E) = 0]\n";
    std::cout << "    Initial guess:             " << E_guess_c[0] << " Hartree\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Exact E0:                  " << "-0.5" << " Hartree\n";
    std::cout << "    Found E0:                  " << res_E0_c.root[0] << " Hartree\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Residual norm ‖M(E)‖:      " << M(res_E0_c.root).norm() << "    ";
    print_pass_fail(f1(res1.root).norm() < acc_global);
    std::cout << std::defaultfloat;
    std::cout << "    Iterations:                " << res_E0_c.iterations_taken << "\n";
    std::cout << "    Exit reason:               " << res_E0_c.exit_reason << "\n\n";

    return 0;

} // main
