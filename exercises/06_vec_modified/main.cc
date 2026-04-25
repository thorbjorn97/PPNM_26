// "main.cc" source file: Hosts main() function for tests
#include <iostream>
#include <random>
#include <iomanip>
#include "vec.h" // "vec.h" header file contain the implementation of the vec class

// Hint 6: "Run extensive tests of your implementation in the main function"
// Extensive tests of vec class operators and methods
// Includes random tests similar to C# example given in Hint 6

template<typename T> // {new: added template parameter T}

void run_tests(const std::string& label) { // {new: reusable test routine for multiple vec<T> instantiations}
    std::cout << "=== Testing vec<" << label << "> ===\n\n"; // {new: header for test section}

    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // generate two random test vectors u and v
    vec<T> u(dis(gen), dis(gen), dis(gen)); // {mod: generalized to vec<T>}
    vec<T> v(dis(gen), dis(gen), dis(gen)); // {mod: generalized to vec<T>}

    std::cout << "u = " << u << "\n";
    std::cout << "v = " << v << "\n\n";

    vec<T> t; // {mod: generalized to vec<T>}

    // unary minus
    t = vec<T>(-u.x, -u.y, -u.z); // {mod: generalized to vec<T>}
    std::cout << "-u = " << -u << "\n"; // operator computation
    std::cout << "t  = " << t  << "\n"; // manual computation

    if(approx(t, -u))
        std::cout << "test 'unary -' passed\n\n";
    else
        std::cout << "test 'unary -' FAILED\n\n";

    // subtraction
    t = vec<T>(u.x - v.x, u.y - v.y, u.z - v.z);
    std::cout << "u-v = " << (u - v) << "\n"; // operator computation
    std::cout << "t   = " << t        << "\n"; // manual computation

    if(approx(t, u - v))
        std::cout << "test 'operator-' passed\n\n";
    else
        std::cout << "test 'operator-' FAILED\n\n";

    // addition
    t = vec<T>(u.x + v.x, u.y + v.y, u.z + v.z);
    std::cout << "u+v = " << (u + v) << "\n"; // operator computation
    std::cout << "t   = " << t        << "\n"; // manual computation

    if(approx(t, u + v))
        std::cout << "test 'operator+' passed\n\n";
    else
        std::cout << "test 'operator+' FAILED\n\n";

    // scalar multiplication
    T c = static_cast<T>(dis(gen));
    t = vec<T>(u.x*c, u.y*c, u.z*c);

    std::cout << "u*c = " << (u*c) << "\n"; // operator computation
    std::cout << "t   = " << t     << "\n"; // manual computation

    if(approx(t, u*c))
        std::cout << "test 'operator*' passed\n\n";
    else
        std::cout << "test 'operator*' FAILED\n\n";

    // scalar division
    T d = static_cast<T>(dis(gen) + 0.1); // avoid zero
    t = vec<T>(u.x/d, u.y/d, u.z/d);

    std::cout << "u/d = " << (u/d) << "\n"; // operator computation
    std::cout << "t   = " << t     << "\n"; // manual computation

    if(approx(t, u/d))
        std::cout << "test 'operator/' passed\n\n";
    else
        std::cout << "test 'operator/' FAILED\n\n";

    // dot product
    T dot_manual = u.x*v.x + u.y*v.y + u.z*v.z; // {mod: generalized to T}
    T dot_operator = u.dot(v); // {mod: generalized to T}

    std::cout << "u·v (operator) = " << dot_operator << "\n"; // operator computation
    std::cout << "u·v (manual)   = " << dot_manual   << "\n"; // manual computation

    if(std::abs(dot_manual - dot_operator) < T(1e-9)) // {mod: generalized to T}
        std::cout << "test 'dot' passed\n\n";
    else
        std::cout << "test 'dot' FAILED\n\n";

    // cross product
    t = vec<T>( // {mod: generalized to vec<T>}
        u.y*v.z - u.z*v.y,
        u.z*v.x - u.x*v.z,
        u.x*v.y - u.y*v.x
    );

    std::cout << "u×v (operator) = " << u.cross(v) << "\n"; // operator computation
    std::cout << "u×v (manual)   = " << t          << "\n"; // manual computation

    if(approx(t, u.cross(v)))
        std::cout << "test 'cross' passed\n\n";
    else
        std::cout << "test 'cross' FAILED\n\n";

    // norm
    T norm_manual = std::sqrt(u.x*u.x + u.y*u.y + u.z*u.z); // {mod: generalized to T}
    T norm_operator = u.norm(); // {mod: generalized to T}

    std::cout << "|u| (operator) = " << norm_operator << "\n"; // operator computation
    std::cout << "|u| (manual)   = " << norm_manual   << "\n"; // manual computation

    if(std::abs(norm_manual - norm_operator) < T(1e-9)) // {mod: generalized to T}
        std::cout << "test 'norm' passed\n\n";
    else
        std::cout << "test 'norm' FAILED\n\n";

    // approx function
    vec<T> a(T(1), T(2), T(3));
    vec<T> b(T(1)+T(1e-7), T(2), T(3));

    std::cout << "approx(a,b) = "
              << (approx(a,b) ? "true" : "false") << "\n\n"; // {mod}

    // return 0; // Returns 0 to indicate successful execution // {rem: only main() should return 0, not this test function}
} // (using Hint 6 for inspiration)

int main() {
    run_tests<double>("double");
    run_tests<float>("float");
    return 0;
} // {mod: replaced inline test code with calls to run_tests<T>() to execute the same test routine for T=double and T=float}

// end