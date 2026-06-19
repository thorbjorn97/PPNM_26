// main.cc

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>


#include "utils.h"
#include "vector.h"
#include "matrix.h"

#include "integrator_mc.h"
#include "test_functions.h"
#include "rng.h"

#include "integrator_qmc.h"
#include "integrator_stratified.h"

using namespace pp;

int main(){
    // ============================================================================================
    // Task A (6 points): Plain Monte Carlo integration
    // ============================================================================================
    section("Task A (6 points): Plain Monte Carlo integration");

    // Global configuration:
    std::cout << std::fixed << std::setprecision(6); // fix output precision
    int N_samples = 1e6;          // set number of sampling points for comparative computations
    int N_samples_loop_min = 20;  // set lower bound on number of sampling points in loops for convergence tests
    int N_samples_loop_max = 1e7; // set upper bound on number of sampling points in loops for convergence tests
    lcg rng_LCG(42);              // (LCG) seed for the linear congruential generator (simple "quick-and-dirty" generator) 
    stl rng_STL(42);              // (STL) seed for the chosen (Mersenne Twister) standard C++ library rng 
    halton seq_Halton({2,3,5});   // Halton sequence generator

    std::cout << "\nGlobal configuration (for Tasks A, B, and C):\n";
    std::cout << "N_samples          = " << N_samples << "\n";
    std::cout << "N_samples_loop_min = " << N_samples_loop_min << "\n";
    std::cout << "N_samples_loop_max = " << N_samples_loop_max << "\n";
    std::cout << "LCG seed           = 42\n";
    std::cout << "STL seed           = 42\n";
    std::cout << "Halton seq base    = {2,3,5}\n\n";

    // ========================================
    // A.1 - IMPLEMENTATION OF PLAIN MULTI-DIMENSIONAL MONTE-CARLO INTEGRATOR
    // ========================================
    substep("A.1 - IMPLEMENTATION OF PLAIN MONTE CARLO MULTI-DIMENSIONAL INTEGRATOR");

    std::cout << "Plain Monte Carlo integrator: See plain_mc() in integrator_mc.cc\n";
    std::cout << "LCG random number generator:  See lcg() in rng.h\n\n";

    // ========================================
    // A.2 - TEST OF INTEGRATOR ON 2D INTEGRALS
    // ========================================
    substep("A.2 - TEST OF INTEGRATOR ON 2D INTEGRALS");

    // (1) Unit circle area (volume under unit circle with height 1 on [-1,1]^2)
    std::cout << "Integral 1: ∫∫ √(1-x²) dxdy on [-1,1]² (Area of a unit circle)\n\n";

    vector a1{-1,-1};  // lower limits
    vector b1{ 1, 1};  // upper limits

    auto circle_result = plain_mc(unit_circle, a1, b1, N_samples, rng_LCG);  // result of MC integration

    std::cout
        << "Exact value  = " << M_PI << "\n"
        << "MC result    = " << circle_result.integral << "\n"
        << "Error        = " << std::abs(circle_result.integral - M_PI) << "\n\n";

    // (2) Gaussian 2D integral on [0,1]^2 (volume under z=exp(-(x^2+y^2)) on [0,1]^2)
    std::cout << "Integral 2: ∫∫ exp(-(x²+y²)) dxdy on [0,1]² (Square section of simple 2D Gaussian)\n\n";

    vector a2{ 0, 0};  // lower limits
    vector b2{ 1, 1};  // upper limits
    
    auto gauss_result = plain_mc(gaussian, a2, b2, N_samples, rng_LCG);  // result of MC integration

    double erf1 = std::erf(1.0);
    double exact_gauss = (M_PI / 4.0) * erf1 * erf1;  // precomputed reference value

    std::cout
        << "Exact value  = " << exact_gauss << "\n"
        << "MC result    = " << gauss_result.integral << "\n"
        << "Error        = " << std::abs(gauss_result.integral - exact_gauss) << "\n\n";

    // ========================================
    // A.4 - PLOT AND ANALYSIS OF ERROR SCALING
    // ========================================
    substep("A.4 - PLOT AND ANALYSIS OF ERROR SCALING");

    std::cout << "Integral used for testing: ∫∫ √(1-x²) dxdy on [-1,1]² (Area of a unit circle)\n\n";
    
    vector a3{-1,-1};  // lower limits
    vector b3{ 1, 1};  // upper limits

    std::ofstream fout("errors_circle.dat");
    for(int N = N_samples_loop_min; N <= N_samples_loop_max; N*=2){
        auto result = plain_mc(unit_circle, a3, b3, N, rng_LCG);
        double estim_error = result.error;
        double actual_error = std::abs(result.integral - M_PI);
        fout
            << N << " "               // number of samples
            << estim_error << " "     // estimated error (should scale as 1/sqrt(N))
            << actual_error << "\n";  // actual error
    }
    fout.close();
    std::cout << "Datafile created: errors_circle.dat\n";

    std::cout << "Plot: See plot.gpi and error_scaling.png\n\n";
    std::cout << "Conclusion: The fit indicates that the actual error indeed scales as 1/√N.\n\n";

    // ========================================
    // A.5 - CALCULATION OF 3D ELLIPSOID VOLUME
    // ========================================
    substep("A.5 - 3D ELLIPSOID VOLUME");

    vector a4{-1,-2,-3};  // lower limits
    vector b4{ 1, 2, 3};  // upper limits

    auto result = plain_mc(ellipsoid, a4, b4, N_samples, rng_LCG);  // result of MC integration
    
    std::cout
        << "Exact value  = " << 8 * M_PI << "\n"
        << "MC result    = " << result.integral << "\n"
        << "Error        = " << std::abs(result.integral - 8 * M_PI) << "\n";

    // ============================================================================================
    // Task B (3 points): Quasi-random sequences
    // ============================================================================================
    section("Task B (3 points): Quasi-random sequences");

    // ========================================
    // B.1 - IMPLEMENTATION OF QUASI-RANDOM SEQUENCE MULTIDIMENSIONAL MONTE-CARLO INTEGRATOR
    // ========================================
    substep("B.1 - IMPLEMENTATION OF QUASI-RANDOM SEQUENCE MULTI-DIMENSIONAL MONTE-CARLO INTEGRATOR");

    std::cout << "See quasi_mc() in integrator_qmc.cc\n\n";

    // ========================================
    // B.2 - COMPARISON OF ERROR SCALING WITH PLAIN MONTE CARLO INTEGRATOR
    // ========================================
    substep("B.2 - COMPARISON OF ERROR SCALING WITH PLAIN MONTE CARLO INTEGRATOR");
    
    std::cout << "STL random number generator:  See stl() in rng.h\n";
    std::cout << "Halton sequence generator:    See halton() in rng.h\n\n";
    std::cout << "Integral used for comparison: ∫∫∫ xyz dxdydz on [0,1]³ (Simple smooth function)\n\n";

    std::ofstream fout_qmc("errors_qmc.dat");

    vector a5{0,0,0};  // lower limits
    vector b5{1,1,1};  // upper limits

    double exact_smooth = 1.0 / 8.0;  // precomputed reference value (∫∫∫ xyz dxdydz on [0,1]³ is 1/8 exactly)
    
    for(int N = N_samples_loop_min; N <= N_samples_loop_max; N *= 2){
        // Note: for each value of N, I use independent realizations of LCG, STL and Halton (from different section of the same seed, this is statistically right)
        auto res_lcg = plain_mc(smooth_test, a5, b5, N, rng_LCG);     // i)   LCG rng
        auto res_stl = plain_mc(smooth_test, a5, b5, N, rng_STL);     // ii)  STL rng
        auto res_qmc = quasi_mc(smooth_test, a5, b5, N, seq_Halton);  // iii) Halton sequence

        fout_qmc
            << N << " "
            << std::abs(res_lcg.integral - exact_smooth) << " "
            << std::abs(res_stl.integral - exact_smooth) << " "
            << std::abs(res_qmc.integral - exact_smooth) << "\n";
    }
    fout_qmc.close();

    std::cout << "Datafile created: errors_qmc.dat\n";
    std::cout << "Plot: See plot.gpi and error_scaling_qmc.png\n\n";
    std::cout << "Conclusion: The fits indicate that:\n";
    std::cout << "- the estimated error scales as 1/√N for LCG and STL\n";
    std::cout << "- the estimated error scales as 1/N for quasi-random sequences (Halton)\n\n";

    // ========================================
    // B.3 - COMPARATIVE TEST ON DIFFICULT SINGULAR INTEGRAL
    // ========================================
    substep("B.3 - COMPARATIVE TEST ON DIFFICULT SINGULAR INTEGRAL");

    vector a6{0, 0, 0};           // lower limits
    vector b6{M_PI, M_PI, M_PI};  // upper limits
    double exact = 1.393203929685676859; // precomputed reference value (from instructions)

    auto res_lcg = plain_mc(singular_integrand, a6, b6, N_samples, rng_LCG);    // i)   LCG rng
    auto res_stl = plain_mc(singular_integrand, a6, b6, N_samples, rng_STL);    // ii)  STL rng
    auto res_qmc = quasi_mc(singular_integrand, a6, b6, N_samples, seq_Halton); // iii) Halton sequence

    // PRINT RESULTS
    std::cout << "RESULTS:\n";

    std::cout << "Exact value        = " << exact << "\n\n";

    std::cout << "Simple LCG         = " << res_lcg.integral
            << "   error = " << std::abs(res_lcg.integral - exact)
            << "   error (est.) = " << res_lcg.error << "\n";
    std::cout << "Standard library   = " << res_stl.integral
            << "   error = " << std::abs(res_stl.integral - exact)
            << "   error (est.) = " << res_stl.error << "\n";
    std::cout << "Quasi-Random       = " << res_qmc.integral
            << "   error = " << std::abs(res_qmc.integral - exact)
            << "   error (est.) = " << "N/A" << "\n";

    //=============================================================================================
    // Task C (1 point): Stratified sampling
    // ============================================================================================
    section("Task C (1 point): Stratified sampling");

    // ========================================
    // C.1 - IMPLEMENTATION OF STRATIFIED SAMPLING ROUTINE
    // ========================================
    substep("C.1 - IMPLEMENTATION OF STRATIFIED SAMPLING ROUTINE");

    std::cout << "See stratified_mc() in integrator_stratified.cc\n\n";

    // ========================================
    // C.2 - TESTING ON DIFFICULT SINGULAR INTEGRAL
    // ========================================
    substep("C.2 - TESTING ON DIFFICULT SINGULAR INTEGRAL");

    int nmin = 500; // minimum block sample limit threshold

    auto res_strat = stratified_mc(singular_integrand, a6, b6, N_samples, nmin, rng_STL);

    std::cout << "Stratified         = " << res_strat.integral
              << "   error = " << std::abs(res_strat.integral - exact)
              << "   error (est.) = " << res_strat.error << "\n\n\n";
   
    return 0;

} // main
