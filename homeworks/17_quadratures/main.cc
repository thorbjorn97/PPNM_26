// main.cc

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "utils.h"
#include "vector.h"
#include "matrix.h"
#include "erf.h"
#include "integrator.h"
#include "cc_integrator.h"

using namespace pp;

int main(){
    // ============================================================================================
    // Task A (6 points): Recursive open 4-point adaptive integrator
    // ============================================================================================
    section("Task A (6 points): Recursive open 4-point adaptive integrator");

    // ========================================
    // A1: IMPLEMENTATION OF INTEGRATOR
    // ========================================
    substep("A.1 - IMPLEMENTATION OF INTEGRATOR");

    std::cout << "See integrate() in integrator.cc\n\n";

    // ========================================
    // A2: TEST OF INTEGRATOR ON FUNCTIONS
    // ========================================
    substep("A.2 - TEST OF INTEGRATOR ON FUNCTIONS");

    double target_acc = 0.001; // matching instructions
    double target_eps = 0.001;

    auto test1 = [](double x){ return std::sqrt(x); };              double ex1 = 2.0/3.0;
    auto test2 = [](double x){ return 1.0/std::sqrt(x); };          double ex2 = 2.0;
    auto test3 = [](double x){ return std::sqrt(1.0 - x*x); };      double ex3 = M_PI/4.0; // mistake in instructions: π/4, not π/2 (given that [a,b]=[0,1] and not [-1,1])
    auto test4 = [](double x){ return std::log(x)/std::sqrt(x); };  double ex4 = -4.0;

    auto r1 = integrate(test1, 0, 1, target_acc, target_eps);
    auto r2 = integrate(test2, 0, 1, target_acc, target_eps);
    auto r3 = integrate(test3, 0, 1, target_acc, target_eps);
    auto r4 = integrate(test4, 0, 1, target_acc, target_eps);

    std::cout << "\nConfiguration: [a, b, acc, eps] = [0, 1, " << target_acc << ", " << target_eps << "] (as per instructions)\n";
    std::cout << "Acc goal is met when: |Computed - Exact| <= acc + eps * |Computed|\n";

    std::cout << std::string(75, '-') << "\n";
    std::cout << std::setprecision(6);
    std::cout << std::left
              << std::setw(17) << "Function" 
              << std::setw(15) << "Computed" 
              << std::setw(15) << "Exact" 
              << std::setw(15) << "Real Error" 
              << "Acc goal met?\n";
    std::cout << std::string(75, '-') << "\n";

    auto verify = [&](std::string name, double comp, double exact) {
        double real_err = std::abs(comp - exact);
        double tol = target_acc + target_eps * std::abs(comp);
        std::string status = (real_err <= tol) ? "YES" : "NO";
        std::cout << std::left << std::setw(17) << name 
                  << std::setw(15) << std::fixed << std::setprecision(10) << comp
                  << std::setw(15) << std::fixed << std::setprecision(10) << exact
                  << std::setw(15) << std::scientific << std::setprecision(6) << real_err
                  << std::defaultfloat << status << "\n";
    };

    verify("sqrt(x)", r1.value, ex1);
    verify("1/sqrt(x)", r2.value, ex2);
    verify("sqrt(1-x^2)", r3.value, ex3);
    verify("log(x)/sqrt(x)", r4.value, ex4);
    std::cout << "\n";

    // ========================================
    // A.3 - IMPLEMENTATION OF ERF
    // ========================================
    substep("A.3 - IMPLEMENTATION OF ERF");

    std::cout << "See erf_integral() in erf.cc\n\n";

    // ========================================
    // A.4 - PLOT OF IMPLEMENTED ERF
    // ========================================
    substep("A.4 - PLOT OF IMPLEMENTED ERF");

    std::ofstream out1("erf_function.dat");
    // Sample full transformation coordinate ranges
    for(double z = -4.0; z <= 4.0; z += 0.005)
    {
        double val = erf_integral(z, 1e-3, 1e-3);
        double exact = std::erf(z); // tabulated reference (C++ standard)
        double err = std::abs(val - exact);
        double tol = target_acc + target_eps * std::abs(val);
        out1 << z << " " << val << " " << exact << " " << err << " " << tol << "\n";
    }
    out1.close();
    std::cout << "Datafile created: erf_function.dat\n";
    std::cout << "Plotting: See plot.gpi and erf_function.png\n\n";

    // ========================================
    // A.5 - COMPARISON WITH TABULATED VALUES
    // ========================================
    substep("A.5 - COMPARISON WITH TABULATED VALUES");

    // Explicitly printing comparison as requested ("compare with tabulated values")
    std::cout << "\nConfiguration: [acc, eps] = [0.001, 0.001]\n";
    std::cout << "Tabulated values: std::erf (inbuilt C++ standard)\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << std::left
              << std::setw(10) << "z" 
              << std::setw(18) << "erf_integral(z)" 
              << std::setw(18) << "std::erf(z)"
              << "Error\n";
    std::cout << std::string(60, '-') << "\n";

    for(double z : {0.0, 0.5, 1.0, 2.0, 3.0}) {
        double erf_impl = erf_integral(z, 0.001, 0.001);
        double erf_inbuilt = std::erf(z);
        double err = std::abs(erf_impl - erf_inbuilt);
        std::cout << std::left << std::fixed << std::setprecision(1) << std::setw(10) << z 
                  << std::setw(18) << std::fixed << std::setprecision(10) << erf_impl
                  << std::setw(18) << std::fixed << std::setprecision(10) << erf_inbuilt
                  << std::scientific << std::setprecision(6) << err << "\n";
    }
    std::cout << std::defaultfloat << "\n";

    // ========================================
    // A.6 - CONVERGENCE STUDY FOR ERF(1.0)
    // ========================================
    substep("A.6 - CONVERGENCE STUDY FOR ERF(1.0)");

    const double exact_erf1 = 0.84270079294971486934;

    std::ofstream out2("erf_convergence.dat");
    for(int k = 1; k <= 10; k++)
    {
        double acc_k = std::pow(10.0, -k);
        double val = erf_integral(1.0, acc_k, 0.0);
        double err = std::abs(val - exact_erf1);
        out2 << acc_k << " " << err << "\n";
    }
    out2.close();
    std::cout << "Datafile created: erf_convergence.dat\n";
    std::cout << "Plotting: See plot.gpi and erf_convergence.png\n";
    
    // ============================================================================================
    // Task B (3 points): Variable transformation quadratures
    // ============================================================================================
    section("Task B (3 points): Variable transformation quadratures");

    // ========================================
    // B1: IMPLEMENTATION OF CLENSHAW-CURTIS INTEGRATOR
    // ========================================
    substep("B.1 - IMPLEMENTATION OF CLENSHAW-CURTIS INTEGRATOR");

    std::cout << "See integrate_cc() in cc_integrator.cc\n\n";

    // ========================================
    // B2: PYTHON DATA PREPARATION
    // ========================================
    substep("B.2 - PYTHON DATA PREPARATION");
    
    // Programmatically invoke the Python script to run live benchmarks
    int py_status = std::system("python3 compare.py");
    if (py_status != 0) {
        std::cerr << "Error: python3 compare.py failed to execute.\n";
    }

    // Read back the live Python data from the generated file
    std::ifstream py_file("python_results.txt");
    double py_val1 = 0, py_err1 = 0, py_val2 = 0, py_err2 = 0, py_val3 = 0, py_err3 = 0;
    int py_calls1 = 0, py_calls2 = 0, py_calls3 = 0;

    if (py_file.is_open()) {
        py_file >> py_val1 >> py_err1 >> py_calls1;
        py_file >> py_val2 >> py_err2 >> py_calls2;
        py_file >> py_val3 >> py_err3 >> py_calls3;
        py_file.close();
    } else {
        std::cerr << "Warning: Could not read python_results.txt, displaying 0s.\n";
    }

    std::cout << "See compare.py\n\n";

    // ========================================
    // B3: TEST ON INTEGRALS WITH INTEGRABLE DIVERGENCIES
    // ========================================
    substep("B.3 - TEST ON INTEGRALS WITH INTEGRABLE DIVERGENCIES");

    // Integrable divergence integral 1: ∫ 1/sqrt(x) dx on [0, 1]
    std::cout << "\nComparative test on integral: ∫ 1/sqrt(x) dx on [0, 1]" << "\n";

    auto f1 = [](double x) { return 1.0 / std::sqrt(x); };
    const double exact1 = 2.0;

    auto b1 = integrate(f1, 0.0, 1.0);
    int calls_b1 = get_integration_calls();

    auto b2 = integrate_cc(f1, 0.0, 1.0);
    int calls_b2 = get_integration_calls();

    std::cout << std::string(75, '-') << "\n";
    std::cout << std::left << std::setw(22) << "Method" << std::setw(16) << "Value" << std::setw(16) << "Error" << "Integrand Evaluations\n";
    std::cout << std::string(75, '-') << "\n";
    
    std::cout << std::setw(22) << "integrate (ordinary):" << std::fixed << std::setprecision(10) << std::setw(16) << b1.value << std::scientific << std::setprecision(6) << std::setw(16) << std::abs(b1.value - exact1) << std::defaultfloat << calls_b1 << "\n";
    std::cout << std::setw(22) << "integrate_cc (CC):"   << std::fixed << std::setprecision(10) << std::setw(16) << b2.value << std::scientific << std::setprecision(6) << std::setw(16) << std::abs(b2.value - exact1) << std::defaultfloat << calls_b2 << "\n";
    std::cout << std::setw(22) << "quad (Python):"    << std::fixed << std::setprecision(10) << std::setw(16) << py_val1  << std::scientific << std::setprecision(6) << std::setw(16) << py_err1               << std::defaultfloat << py_calls1 << "\n\n";

    // Integrable divergence integral 2: ∫ ln(x)/sqrt(x) dx on [0, 1]
    std::cout << "\nComparative test on integral: ∫ ln(x)/sqrt(x) dx on [0, 1]" << "\n";

    auto f2_func = [](double x) { return std::log(x) / std::sqrt(x); };
    const double exact2 = -4.0;

    auto b3 = integrate(f2_func, 0.0, 1.0);
    int calls_b3 = get_integration_calls();

    auto b4 = integrate_cc(f2_func, 0.0, 1.0);
    int calls_b4 = get_integration_calls();

    std::cout << std::string(75, '-') << "\n";
    std::cout << std::left << std::setw(22) << "Method" << std::setw(16) << "Value" << std::setw(16) << "Error" << "Integrand Evaluations\n";
    std::cout << std::string(75, '-') << "\n";

    std::cout << std::setw(22) << "integrate (ordinary):" << std::fixed << std::setprecision(10) << std::setw(16) << b3.value << std::scientific << std::setprecision(6) << std::setw(16) << std::abs(b3.value - exact2) << std::defaultfloat << calls_b3 << "\n";
    std::cout << std::setw(22) << "integrate_cc (CC):"   << std::fixed << std::setprecision(10) << std::setw(16) << b4.value << std::scientific << std::setprecision(6) << std::setw(16) << std::abs(b4.value - exact2) << std::defaultfloat << calls_b4 << "\n";
    std::cout << std::setw(22) << "quad (Python):"    << std::fixed << std::setprecision(10) << std::setw(16) << py_val2  << std::scientific << std::setprecision(6) << std::setw(16) << py_err2               << std::defaultfloat << py_calls2 << "\n\n";

    // ========================================
    // B4: GENERALIZATION TO INFINITE INTERVALS
    // ========================================
    substep("B.4 - GENERALIZATION TO INFINITE INTERVALS");

    std::cout << "See integrate_cc_inf() in cc_integrator.cc\n\n";

    // ========================================
    // B5: TEST ON INFINITE LIMIT INTEGRAL
    // ========================================
    substep("B.5 - TEST ON INFINITE LIMIT INTEGRAL");

    // Infinite limit integral: ∫ exp(-x^2) dx on (-inf, +inf)
    std::cout << "\nComparative test on integral: ∫ exp(-x^2) dx on (-inf, +inf)" << "\n";

    auto gauss = [](double x) { return std::exp(-x * x); };
    const double exact3 = std::sqrt(M_PI);

    auto b5 = integrate_cc_inf(gauss);
    int calls_b5 = get_integration_calls();

    std::cout << std::string(75, '-') << "\n";
    std::cout << std::left << std::setw(22) << "Method" << std::setw(16) << "Value" << std::setw(16) << "Error" << "Integrand Evaluations\n";
    std::cout << std::string(75, '-') << "\n";
    
    std::cout << std::setw(22) << "integrate (ordinary):" << std::setw(16) << "N/A" << std::setw(16) << "N/A" << "N/A\n";
    std::cout << std::setw(22) << "integrate_cc_inf:"  << std::fixed << std::setprecision(10) << std::setw(16) << b5.value << std::scientific << std::setprecision(6) << std::setw(16) << std::abs(b5.value - exact3) << std::defaultfloat << calls_b5 << "\n";
    std::cout << std::setw(22) << "quad (Python):"    << std::fixed << std::setprecision(10) << std::setw(16) << py_val3  << std::scientific << std::setprecision(6) << std::setw(16) << py_err3               << std::defaultfloat << py_calls3 << "\n";
    
    // Clean up temporary data pipeline exchange file
    std::remove("python_results.txt");

    // ============================================================================================
    // Task C (1 point): Error estimate
    // ============================================================================================
    section("Task C (1 point): Error estimate");

    substep("C.1 - ADJUSTMENT OF INTEGRATOR TO PROVIDE ERROR ESTIMATE");
    
    std::cout << "See within adapt() in integrator.cc\n\n";

    substep("C.2 - EVALUATION OF ESTIMATED VS ACTUAL ERROR ON A DIFFICULT INTEGRAL");
    
    // High-precision analytical baseline for comparison
    const double exact_c = -0.0489888171154; // From Desmos
    auto f_difficult = [](double x) {
        if (x == 0.0) return 0.0; // Avoid nan at exact zero boundary
        return std::log(x) * std::cos(10.0 * M_PI * x);
    };

    std::cout << "\nEvaluation on integral: ∫ ln(x) * cos(10 * pi * x) dx on [0, 1]\n";
    // This integral is difficult due to combining an endpoint singularity with rapid oscillations

    std::cout << std::string(75, '-') << "\n";
    std::cout << std::left << std::setw(12) << "Target Acc" 
              << std::setw(16) << "Value" 
              << std::setw(16) << "Estim. Error" 
              << std::setw(16) << "Actual Error" 
              << "Ratio (Est/Act)\n";
    std::cout << std::string(75, '-') << "\n";

    // Sweep across a spectrum of accuracies to monitor the scaling behavior
    for (double acc_c : {1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7}) {
        // Run with eps = 0 to isolate absolute accuracy behavior
        auto r = integrate(f_difficult, 0.0, 1.0, acc_c, 0.0); 
        
        double actual_error = std::abs(r.value - exact_c);
        double ratio = (actual_error > 0.0) ? (r.error / actual_error) : 1.0;

        std::cout << std::left << std::scientific << std::setprecision(2)
                  << std::setw(12) << acc_c
                  << std::fixed      << std::setprecision(8) << std::setw(16) << r.value
                  << std::scientific << std::setprecision(4) 
                  << std::setw(16) << r.error
                  << std::setw(16) << actual_error
                  << std::fixed      << std::setprecision(2) << ratio << "\n";
    }
    std::cout << "\n" << std::defaultfloat;

    return 0;
    
}
