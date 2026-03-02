// "main" source file
#include <iostream>
#include <cmath> // for std::abs
#include <limits>
#include <iomanip>

// ----- Task 3: Comparing doubles
// Implement approx function
bool approx(double a, double b, double acc = 1e-9, double eps = 1e-9) {
    double diff = std::abs(a - b);
    if (diff <= acc) return true;  // absolute tolerance
    double max_ab = std::max(std::abs(a), std::abs(b));
    return diff <= eps * max_ab;   // relative tolerance
}
// The above function returns False if both statements below are False:
// 1) |a-b| ≤ acc
// 2) |a-b|/Max(|a|,|b|) ≤ eps.
// Otherwise it returns True.

int main() {
// ----- Task 1: Machine epsilon
std::cout << "\n=== [Task 1] Machine epsilon ===\n\n";
// Machine epsilon is computed in 3 different ways. Results are compared.
std::cout << "-- Method 1: Empirical calculation (via while-loop) --\n";
float       f=1.0f; while((float)      (1.0f+f) != 1.0f){f/=2.0f;} f*=2.0f;
double      d=1.0d; while((double)     (1.0d+d) != 1.0d){d/=2.0d;} d*=2.0d;
long double l=1.0L; while((long double)(1.0L+l) != 1.0L){l/=2.0L;} l*=2.0L;
std::printf("      float eps = %g\n",f);
std::printf("     double eps = %g\n",d);
std::printf("long double eps = %Lg\n",l);

std::cout << "-- Method 2: Library definition (from <limits>) --\n";
std::cout << "      float eps = " << std::numeric_limits<float>::epsilon() << "\n";
std::cout << "     double eps = " << std::numeric_limits<double>::epsilon() << "\n";
std::cout << "long double eps = " << std::numeric_limits<long double>::epsilon() << "\n";

std::cout << "-- Method 3: Theoretical value (via std::pow(2,-n) for n=23,52,63) --\n";
std::cout << "      float eps = " << std::pow(2,-23) << "\n"; // should match float (it does)
std::cout << "     double eps = " << std::pow(2,-52) << "\n"; // should match double (it does)
std::cout << "long double eps = " << std::pow(2,-63) << "\n"; // should match long double (it does)
std::cout << "-- Conclusion --\n";
std::cout << "All 3 computation methods agree on the machine epsilon of float, double and long double.\n";

// ----- Task 2: Non-commutativity of addition (Non-associativity of floating-point addition)
std::cout << "\n=== [Task 2] Non-commutativity of addition ===\n\n";
std::cout << "-- Definitions --\n";
std::cout << "epsilon = std::pow(2,-52)" << "\n";
std::cout << "   tiny = epsilon/2" << "\n";
std::cout << "      a = 1 + tiny + tiny" << "\n";
std::cout << "      b = tiny + tiny + 1" << "\n";

double epsilon = std::pow(2,-52); // epsilon is defined as the double machine epsilon 
double tiny = epsilon/2; // tiny is defined as half of the double machine epsilon
double a = 1 + tiny + tiny; // Expectation: a should be 1+epsilon, that is, larger than 1, no?
double b = tiny + tiny + 1; // Expectation: b should be the same mathematically, no?

// Testing commutativity of a and b (actually, we are testing associativity)
std::cout << "-- Tests --\n";
std::cout << "a==b ?  " << (a==b ? "true":"false") << "\n";
std::cout << "a>1  ?  " << (a>1  ? "true":"false") << "\n";
std::cout << "b>1  ?  " << (b>1  ? "true":"false") << "\n";
// CONCLUSION: The results are different, with a < b.
// Specifically, a == 1 while b == 1 + epsilon, so only b satisfy the mathematical expectation.
// This is clear from the following:
// First, we ensure that floating-point values are printed with 17 decimal digits,
// sufficient to expose the full precision of a double:
std::cout << std::fixed << std::setprecision(17);
// Then, tiny, a and b are printed with 17 digits after the decimal point:
std::cout << "-- Investegation --\n";
std::cout << "               tiny = " << tiny << "\n";
std::cout << "a = 1 + tiny + tiny = " << a << "\n";
std::cout << "b = tiny + tiny + 1 = " << b << "\n";

std::cout << "-- Explanation --\n";
std::cout << "An explanation is given in main.cc\n";
// EXPLANATION: a and b are not equal because floating-point addition is not associative;
// In a, the expression a' = (1 + tiny) is evaluated first, then a = (a' + tiny).
// Since tiny = epsilon/2, tiny is smaller than the spacing between representable numbers
// near 1. Therefore, a' = (1 + tiny) rounds back to 1. For the same reason, the second
// addition a = (a' + tiny) = (1 + tiny) also rounds back to 1.
// Therefore, a = (1 + tiny + tiny) gives 1, leading to a==1 being True;
//
// In b, the expression b' = (tiny + tiny) is evaluated first, then b = (b' + 1).
// Since tiny + tiny is evaluated first, it gives epsilon exactly (since tiny is
// exactly 2^(-53), making the result, 2*2^(-53)=2^(-52) exactly representable without rounding).
// In contrast to tiny, epsilon is not smaller than the spacing between representable numbers
// near 1. Therefore, b = (b' + 1) = (epsilon + 1) produces b = 1 + epsilon.
// Therefore, b = 1 + epsilon, leading to b > 1 being True.
//
// This demonstrates that floating-point addition is not associative.


// ----- Task 3 (continued): Comparing doubles
std::cout << "\n=== [Task 3] Comparing doubles ===\n\n";
std::cout << "== Introduction ==\n";

std::cout << "-- Definitions --\n";
std::cout << " d1 = 0.1+0.1+0.1+0.1+0.1+0.1+0.1+0.1" << "\n";
std::cout << " d2 = 8*0.1" << "\n";

double d1 = 0.1+0.1+0.1+0.1+0.1+0.1+0.1+0.1;
double d2 = 8*0.1;
double acc = 1e-9;
double eps = 1e-9;
double diff = std::abs(a - b);
double max_ab = std::max(std::abs(a), std::abs(b));

std::cout << "-- Test of equality --\n";
std::cout << "d1==d2?  " << (d1==d2 ? "true":"false") << "\n"; 

std::cout << "-- Investegation --\n";
std::cout << std::fixed << std::setprecision(17);
std::cout << "d1 = " << d1 << "\n";
std::cout << "d2 = " << d2 << "\n";

std::cout << "-- Explanation --\n";
std::cout << "Since 0.1 cannot be represented exactly as a 52-digit binary number, d1==d2 return False. \n\n";

std::cout << "== Comparison using approx function ==\n";
std::cout << "-- Definitions --\n";
std::cout << "                 acc = " << acc << "\n";
std::cout << "                 eps = " << eps << "\n";
std::cout << "             |d1-d2| = " << diff << "\n";
std::cout << "eps * Max(|d1|,|d2|) = " << eps * max_ab << "\n";

std::cout << "-- Test of individual conditions --\n";
std::cout << "|d1-d2|≤acc?                 " << (diff<=acc ? "true":"false") << "\n"; 
std::cout << "|d1-d2|≤eps*Max(|d1|,|d2|)?  " << (diff<=eps * max_ab ? "true":"false") << "\n"; 

std::cout << "-- Combined test using approx function --\n";
std::cout << "|d1-d2|≤acc OR |d1-d2|≤eps*Max(|d1|,|d2|) ? " << (approx(d1,d2,acc,eps) ? "true":"false") << "\n"; 

std::cout << "-- Conclusion --\n";
std::cout << "The approx function works as expected on the given example.\n\n";

}

// end
