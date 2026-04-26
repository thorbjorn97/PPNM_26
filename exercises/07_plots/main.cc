// "main.cc" source file: Hosts the main() function

#include <cmath>        // Provides std::lgamma, std::abs
#include <fstream>      // Provides std::ofstream
#include <iostream>     // Provides std::cout, std::fixed
#include <iomanip>      // Provides std::setprecision
#include "functions.h"  // Provides erf, sgamma, and lngamma functions

int main(){
    // === Task 1 === error function approximation, erf(x)
    std::cout << "\n=== Task 1: error function approximation, erf(x) ===" << std::endl;
    std::ofstream erf_out("erf.dat"); // open data file for writing

    // Compute approximated (x, erf(x)) points and write them to data file for plotting 
    for(double x = -3; x <= 3; x += 0.005){
        erf_out << x << " " << erf(x) << "\n";
    }

    erf_out << "\n\n"; // separate sections in data file

    // Store tabulated reference values (from Wikipedia)
    double xtab_erf[] = {-2.0, -1.5, -1.0, -0.5, 0.0, 0.5, 1.0, 1.5, 2.0};
    double ytab_erf[] = {
        -0.9953222650,
        -0.9661051465,
        -0.8427007929,
        -0.5204998778,
        0.0000000000,
        0.5204998778,
        0.8427007929,
        0.9661051465,
        0.9953222650
    };

    // Write tabulated reference values to data file for plotting
    for(int i = 0; i < 9; ++i){
        erf_out << xtab_erf[i] << " " << ytab_erf[i] << "\n";
    }

    erf_out << "\n\n"; // separate sections in data file

    // Calculate approximation errors compared to tabulated values and print results
    double erf_tol = 1e-6;  // error threshold

    for(int i = 0; i < 9; ++i){ // loop over tabulated points
        double approx = erf(xtab_erf[i]);
        double exact  = ytab_erf[i];
        double err = std::abs(approx - exact);

        std::cout << "x = "
                << std::fixed << std::setprecision(1) << xtab_erf[i]
                << std::fixed << std::setprecision(10) // reset formatting
                << " | approx = " << approx << " | tabulated = " << exact << " | error = " << err;

        if(err < erf_tol)
            std::cout << "  [ok]\n";
        else
            std::cout << "  [fail: error >= erf_tol = " << erf_tol << "]\n";
    }

    // === Task 2 === gamma function approximation, Γ(x)
    std::cout << "\n=== Task 2: gamma function approximation, Γ(x) ===" << std::endl;
    std::ofstream gamma_out("gamma.dat"); // open data file for writing

    // Compute approximated (x, Γ(x)) points and write them to data file for plotting
    for(double x = -5; x <= 5; x += 0.005){
        gamma_out << x << " " << sgamma(x) << "\n";
    }

    gamma_out << "\n\n"; // separate sections in data file

    // Tabulated reference values (from factorial check: Γ(n) = (n-1)!)
    double xtab_gamma[] = {1.0, 2.0, 3.0, 4.0};
    double ytab_gamma[] = {
        1.0000000000,        // Γ(1) = (1-1)! = 0! = 1
        1.0000000000,        // Γ(2) = (2-1)! = 1! = 1
        2.0000000000,        // Γ(3) = (3-1)! = 2! = 2
        6.0000000000,        // Γ(4) = (4-1)! = 3! = 6
    };

    for(int i = 0; i < 4; ++i){
        gamma_out << xtab_gamma[i] << " " << ytab_gamma[i] << "\n";
    }

    gamma_out << "\n\n"; // separate sections in data file

    // Calculate approximation errors compared to tabulated values and print results
    double gamma_tol = 1e-6;  // error threshold

    for(int i = 0; i < 4; ++i){ // loop over tabulated points
        double approx = sgamma(xtab_gamma[i]);
        double exact  = ytab_gamma[i];
        double err = std::abs(approx - exact); // error

        std::cout << "x = "
                << std::fixed << std::setprecision(1) << xtab_gamma[i]
                << std::fixed << std::setprecision(10) // reset formatting
                << " | approx = " << approx << " | tabulated = " << exact << " | error = " << err;

        if(err < gamma_tol)
            std::cout << "  [ok]\n";
        else
            std::cout << "  [fail: error >= gamma_tol = " << gamma_tol << "]\n";
    }

    // === Task 3 === log gamma function approximation, ln(Γ(x))
    std::cout << "\n=== Task 3: log gamma function approximation, ln(Γ(x)) ===" << std::endl;
    std::ofstream lngamma_out("lngamma.dat"); // open data file for writing

    // Compute approximated (x, ln(Γ(x))) points and write them to data file for plotting
    for(double x = 0; x <= 15; x += 0.005){
        lngamma_out << x << " " << lngamma(x) << "\n";
    }

    lngamma_out << "\n\n"; // separate sections in data file

    // tabulated reference values (from std::lgamma(n))
    for(int n = 1; n <= 15; ++n){
        lngamma_out << n << " " << std::lgamma(n) << "\n";
    }

    lngamma_out << "\n\n"; // separate sections in data file

    // Calculate approximation errors compared to tabulated values and print results
    double lngamma_tol = 1e-6;  // error threshold

    for(int n = 1; n <= 15; ++n){ // loop over tabulated points
        double approx = lngamma(n);
        double exact  = std::lgamma(n);
        double err = std::abs(approx - exact); // error

        std::cout << "x = "
                << std::fixed << std::setprecision(1) << n
                << std::fixed << std::setprecision(10) // reset formatting
                << " | approx = " << approx << " | tabulated = " << exact << " | error = " << err;

        if(err < lngamma_tol)
            std::cout << "  [ok]\n";
        else
            std::cout << "  [fail: error >= lngamma_tol = " << lngamma_tol << "]\n";
    }

    // close data files after writing is done:
    erf_out.close();
    gamma_out.close();
    lngamma_out.close();

    return 0;
}

// end