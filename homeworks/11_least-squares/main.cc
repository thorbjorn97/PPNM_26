// main.cc

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "lsfit.h"
#include "matrix.h"
#include "vector.h"
#include "utils.h"

using namespace pp;

int main(){
    // ============================================================================================
    // Task A (6 points): Ordinary least-squares fit by QR-decomposition
    // ============================================================================================
    section("Task A (6 points): Ordinary least-squares fit by QR-decomposition");

    // ====================================================
    // INPUT
    // ====================================================
    substep("INPUT");
    vector t  = {1, 2, 3, 4, 6, 9, 10, 13, 15};
    vector y  = {117, 100, 88, 72, 53, 29.5, 25.2, 15.2, 11.1};
    vector dy = {6, 5, 4, 4, 4, 3, 3, 2, 2};

    print_vector(t, "Vector t", 0);
    print_vector(y, "Vector y", 1); 
    print_vector(dy, "Vector dy", 0);

    // ====================================================
    // LOGARITHMIC TRANSFORMATION
    // ====================================================
    // y = a*exp(-lambda*t) --> Y = ln(y) = ln(a) - lambda*t

    vector Y(y.size());  // vector to contain ln(y)
    vector dY(y.size()); // vector to contain dy/y (ln transformed uncertainty)

    // Do ln transformation of y data (y-->Y) and uncertainty (dy-->dY)
    for (size_t i = 0; i < y.size(); i++) {
        Y[i]  = std::log(y[i]);
        dY[i] = dy[i] / y[i]; // Uncertainty: δln(y)=δy/y
    }
    /* PROOF: δln(y)=δy/y
    Using the calculus approach, the uncertainty δY of a function Y = f(y) is:
        δY = |dY/dy|*δy
    For Y = ln(y), this gives (since d/dy ln(y) = 1/y):
        δY = δln(y) = |d/dy ln(y)|*δy = |1/y|*δy = δy/y (since y = activity > 0)
    */

    // ====================================================
    // MODEL DEFINITION
    // ====================================================

    auto fs = std::vector<std::function<double(double)>>{
        [](double)   { return 1.0; },
        [](double x) { return x; }
    };

    // ====================================================
    // ORDINARY LEAST-SQUARES FIT BY QR-DECOMPOSITION
    // ====================================================

    vector c = lsfit(fs,t,Y,dY); // perform the fit

    const double c0 = c[0];  // extract c[0] = ln(a)
    const double c1 = c[1];  // extract c[1] = lambda

    // ====================================================
    // PHYSICAL INTERPRETATION
    // ====================================================

    double a = std::exp(c0);  // activity at t=0
    double lambda = -c1;      // [1/day] decay constant
    double T_half = std::log(2.0) / lambda; // [days] half-life

    double T_half_ref = 3.6316; // [days] 224Ra half-life reference
    double sigma_T_half_ref = 0.0023; // [days] 224Ra half-life reference uncertainty
    double rel_err = 100 * std::abs(T_half - T_half_ref) / T_half_ref; // relative error
    
    // ====================================================
    // RESULTS
    // ====================================================
    substep("RESULTS");

    std::cout << "ĉ0 = ln(a)       = " << c0 << "\n";
    std::cout << "ĉ1 = -lambda     = " << c1 << "\n\n";

    std::cout << "a                = " << a << "\n";
    std::cout << "lambda           = " << lambda << " [1/day]\n";
    std::cout << "T1/2 (fit)       = " << T_half << " days\n";
    std::cout << "T1/2 (modern)    = " << T_half_ref << " days\n";
    std::cout << "rel deviation    = " << rel_err << " % \n";

    // ====================================================
    // DATAFILE FOR PLOT
    // ====================================================
    substep("DATAFILE FOR PLOT");

    // Write experimental data to file ThX.dat
    std::ofstream out_data("ThX.dat");
    for(size_t i = 0; i < t.size(); i++){
        out_data << t[i] << " " << y[i] << " " << dy[i] << "\n";
    }
    std::cout << "Datafile created: ThX.dat\n";

    // Write fitted curve data to file fit.dat
    std::ofstream out_fit("fit.dat");
    for(double tf = 0; tf <= 15; tf += 0.1){
        out_fit << tf << " " << std::exp(c0 + c1 * tf) << "\n";
    }
    std::cout << "Datafile created: fit.dat\n";

    // ====================================================
    // PLOT
    // ====================================================
    substep("PLOT");

    std::cout << "See plot script:  plot.pgi\n";
    std::cout << "See plot:         ThX_fit_A.svg\n";

    // ============================================================================================
    // Task B (3 points): Uncertainties of the fitting coefficients
    // ============================================================================================
    section("Task B (3 points): Uncertainties of the fitting coefficients");

    // Perform fit + covariance matrix by QR-decomposition with modified lsfit
    auto result = lsfit_mod(fs, t, Y, dY);

    // Extract fitting coefficients and covariance matrix
    vector c_B = result.c;
    matrix cov = result.cov;

    // Extract uncertainties from covariance matrix
    double sigma_c0 = std::sqrt(cov(0,0));
    double sigma_c1 = std::sqrt(cov(1,1));

    // Propagate uncertainties into T1/2 (fit)
    double sigma_lambda = sigma_c1;
    double sigma_T_half = std::log(2.0) * sigma_lambda / (lambda * lambda);

    // Check consistency of T1/2 (fit) with T1/2 (modern)
    double diff = std::abs(T_half - T_half_ref);
    bool consistent = diff <= sigma_T_half;

    // ====================================================
    // RESULTS INCLUDING UNCERTAINTIES
    // ====================================================
    substep("RESULTS INCLUDING UNCERTAINTIES");

    std::cout << "ĉ0 = ln(a)       = " << c0 << " ± " << sigma_c0 << "\n";
    std::cout << "ĉ1 = -lambda     = " << c1 << " ± " << sigma_c1 << "\n\n";

    std::cout << "a                = " << std::exp(c0) << " ± " << std::exp(c0) * sigma_c0 << "\n";
    std::cout << "lambda           = " << lambda << " ± " << sigma_lambda << " [1/day]\n";
    std::cout << "T1/2 (fit)       = " << T_half << " ± " << sigma_T_half << " days\n";
    std::cout << "T1/2 (modern)    = " << T_half_ref << " ± " << sigma_T_half_ref << " days\n";
    std::cout << "rel deviation    = " << rel_err << " % \n";

    substep("COMPARISON");
    std::cout << "ΔT (deviation)   = " << diff << " days\n";
    std::cout << "δT (uncertainty) = " << sigma_T_half << " days\n";
    
    std::cout << "Consistency (ΔT < δT ?): " << (consistent ? "YES" : "NO") << "\n";    
    
    // ====================================================
    // CONCLUSION
    // ====================================================
    substep("CONCLUSION");
    if (consistent) {
        std::cout << "T1/2 (modern) lies within the uncertainty region of T1/2 (fit).\n";
    } else {
        std::cout << "T1/2 (modern) lies outside the uncertainty region of T1/2 (fit).\n";
    }

    // ============================================================================================
    // Task C (1 point): Evaluation of the quality of the uncertainties of the fit coefficients
    // ============================================================================================
    section("Task C (1 point): Evaluation of the quality of the uncertainties");
    
    // ====================================================
    // DATAFILE FOR PLOT
    // ====================================================
    substep("DATAFILE FOR PLOT");

    std::ofstream out_bounds("fit_bounds.dat");
    
    // Define the 4 combinations of signs: (++, +-, -+, --)
    double signs[4][2] = { {1.0, 1.0}, {1.0, -1.0}, {-1.0, 1.0}, {-1.0, -1.0} };

    // Loop through an evaluation grid from t = 0 to 15 days
    for (double tf = 0; tf <= 15; tf += 0.1) {
        out_bounds << tf; // write time value tf to file fit_bounds.dat
        
        // Compute the curve value for each of the 4 combinations
        for (int k = 0; k < 4; k++) {
            double c0_var = c0 + signs[k][0] * sigma_c0;
            double c1_var = c1 + signs[k][1] * sigma_c1;
            
            // Transform back out of the logarithm: y = exp(c0 + c1*t)
            double y_var = std::exp(c0_var + c1_var * tf);
            out_bounds << " " << y_var; // write curve value y_var to file fit_bounds.dat
        }
        out_bounds << "\n";
    }
    out_bounds.close();
    std::cout << "Datafile created: fit_bounds.dat\n";

    // ====================================================
    // PLOT
    // ====================================================
    substep("PLOT");

    std::cout << "See plot script:  plot.pgi\n";
    std::cout << "See plot:         ThX_fit_C.svg\n\n\n";

    return 0;
}
