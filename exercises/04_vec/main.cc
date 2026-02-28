// "main.cc" source file: Hosts main() function for tests
#include <iostream>
#include <random>
#include <iomanip>
#include "vec.h" // "vec.h" header file contain the implementation of the vec class

// Hint 6: "Run extensive tests of your implementation in the main function"
// Extensive tests of vec class operators and methods
// Includes random tests similar to C# example given in Hint 6
int main() {
    // random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // generate two random test vectors u and v
    vec u(dis(gen), dis(gen), dis(gen));
    vec v(dis(gen), dis(gen), dis(gen));

    std::cout << "u = " << u << "\n";
    std::cout << "v = " << v << "\n\n";

    vec t;

    // unary minus
    t = vec(-u.x, -u.y, -u.z);
    std::cout << "-u = " << -u << "\n"; // operator computation
    std::cout << "t  = " << t  << "\n"; // manual computation

    if(approx(t, -u))
        std::cout << "test 'unary -' passed\n\n";
    else
        std::cout << "test 'unary -' FAILED\n\n";

    // subtraction
    t = vec(u.x - v.x, u.y - v.y, u.z - v.z);
    std::cout << "u-v = " << (u - v) << "\n"; // operator computation
    std::cout << "t   = " << t        << "\n"; // manual computation

    if(approx(t, u - v))
        std::cout << "test 'operator-' passed\n\n";
    else
        std::cout << "test 'operator-' FAILED\n\n";

    // addition
    t = vec(u.x + v.x, u.y + v.y, u.z + v.z);
    std::cout << "u+v = " << (u + v) << "\n"; // operator computation
    std::cout << "t   = " << t        << "\n"; // manual computation

    if(approx(t, u + v))
        std::cout << "test 'operator+' passed\n\n";
    else
        std::cout << "test 'operator+' FAILED\n\n";

    // scalar multiplication
    double c = dis(gen);
    t = vec(u.x*c, u.y*c, u.z*c);

    std::cout << "u*c = " << (u*c) << "\n"; // operator computation
    std::cout << "t   = " << t     << "\n"; // manual computation

    if(approx(t, u*c))
        std::cout << "test 'operator*' passed\n\n";
    else
        std::cout << "test 'operator*' FAILED\n\n";

    // scalar division
    double d = dis(gen) + 0.1; // avoid zero
    t = vec(u.x/d, u.y/d, u.z/d);

    std::cout << "u/d = " << (u/d) << "\n"; // operator computation
    std::cout << "t   = " << t     << "\n"; // manual computation

    if(approx(t, u/d))
        std::cout << "test 'operator/' passed\n\n";
    else
        std::cout << "test 'operator/' FAILED\n\n";

    // dot product
    double dot_manual = u.x*v.x + u.y*v.y + u.z*v.z;
    double dot_operator = u.dot(v);

    std::cout << "u·v (operator) = " << dot_operator << "\n"; // operator computation
    std::cout << "u·v (manual)   = " << dot_manual   << "\n"; // manual computation

    if(std::abs(dot_manual - dot_operator) < 1e-9)
        std::cout << "test 'dot' passed\n\n";
    else
        std::cout << "test 'dot' FAILED\n\n";

    // cross product
    t = vec(
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
    double norm_manual = std::sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
    double norm_operator = u.norm();

    std::cout << "|u| (operator) = " << norm_operator << "\n"; // operator computation
    std::cout << "|u| (manual)   = " << norm_manual   << "\n"; // manual computation

    if(std::abs(norm_manual - norm_operator) < 1e-9)
        std::cout << "test 'norm' passed\n\n";
    else
        std::cout << "test 'norm' FAILED\n\n";

    // approx function
    vec a(1,2,3);
    vec b(1+1e-7, 2, 3);

    std::cout << "approx(a,b) = "
              << (approx(a,b) ? "true" : "false") << "\n";

    return 0; // Returns 0 to indicate successful execution
} // (using Hint 6 for inspiration)

// end