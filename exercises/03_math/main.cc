// "main" source file
#include <cmath>
#include <complex>
#include <iostream>
#include <numbers> // used for a more accurate value of pi and e
#include "sfuns.h" // sfuns.h and sfuns.cc contain the implementation of the gamma function and its logarithm

using complex = std::complex<double>;
constexpr double PI = std::numbers::pi;
constexpr double E  = std::numbers::e;
constexpr complex I = complex(0,1);

int main(){
    // Task 1: The desired quantities are computed
    std::cout << "=== [Task 1] Computing various quantities ===\n";
    std::cout << "sqrt(2) = " << std::sqrt(2.0)      << "\n";
    std::cout << "2^(1/5) = " << std::pow(2.0,1.0/5) << "\n";
    std::cout << "   e^pi = " << std::pow(E,PI)      << "\n";
    std::cout << "   pi^e = " << std::pow(PI,E)      << "\n";

    std::cout << "    e^i = " << std::exp(I)         << "\n";
    std::cout << "   pi^i = " << std::pow(PI,I)      << "\n";
    std::cout << "    i^i = " << std::pow(I,I)       << "\n";
    std::cout << " log(i) = " << std::log(I)         << "\n\n";

    // Task 2: The Gamma function is computed for n=1..10
    std::cout << "=== [Task 2] Computing Gamma(n) and comparing to factorial ===\n";
    for(int n=1; n<=10; n++){
        double gamma_val = sfuns::fgamma(n);
        double exact    = sfuns::factorial(n-1); // since gamma(n)=factorial(n-1) for positive integers

        std::cout << "n = " << n << ":"
                  << "  Gamma(n) = " << gamma_val
                  << "  (n-1)! = " << exact << "\n";
    }

    // Task 3: The logarithm of the Gamma function is computed for n=1..1000
    std::cout << "\n=== [Task 3] Computing lnGamma(n) for n=10^i where i=0..5 ===\n";
    for(int n=1;n<=100000;n*=10){
        std::cout << "lnGamma("<<n<<") = " << sfuns::lngamma(n) << "\n";
    }

    return 0; // Returns 0 to indicate successful execution
}

// end

// nb: note that you have to justify that the gamma result is correct by comparing it to the factorial (i.e. an exact calculation)
// 