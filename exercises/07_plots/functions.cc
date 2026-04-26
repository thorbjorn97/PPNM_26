// "functions.cc" source file: Implements the functions declared in "functions.h"

#include <vector>      // Provides std::vector
#include <cmath>       // Provides std::log, std::exp, std::sin, M_PI, NAN
#include "functions.h" // To implement the functions declared in the header file

const double PI = M_PI; // define PI constant from cmath

// Below, the functions erf, sgamma, and lngamma are defined exactly as in the exercise description

// === Task 1 === error function approximation
double erf(double x){
// single precision error function (Abramowitz and Stegun, from Wikipedia)
    if(x<0) return -erf(-x);
    
    // coefficients for approximation
    std::vector<double> a {
        0.254829592,-0.284496736,
        1.421413741,-1.453152027,
        1.061405429
    };
    
    double t=1/(1+0.3275911*x);
    double sum=t*(a[0]+t*(a[1]+t*(a[2]+t*(a[3]+t*a[4]))));/* the right thing */
    return 1-sum*std::exp(-x*x);
}

// === Task 2 === gamma function approximation
double sgamma(double x){
    if(x < 0) return PI/std::sin(PI*x)/sgamma(1-x);
    if(x < 9) return sgamma(x+1)/x;

    double lnsgamma =
        std::log(2*PI)/2
        + (x-0.5)*std::log(x)
        - x
        + (1.0/12)/x
        - (1.0/360)/(x*x*x)
        + (1.0/1260)/(x*x*x*x*x);

    return std::exp(lnsgamma);
}

// === Task 3 === log gamma function approximation
double lngamma(double x){
    if(x <= 0) return NAN;
    if(x < 9) return lngamma(x+1) - std::log(x);

    return x*std::log(x + 1/(12*x - 1/x/10))
         - x
         + std::log(2*PI/x)/2;
}

// end