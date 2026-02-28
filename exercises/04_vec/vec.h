// "vec.h" header file: Interface for the class vec (cf. Hint 1)
#pragma once
#include <iostream>
#include <string>

// Task 1: Define a class vec
struct vec {
    // Task 1: Let each instance of vec hold three double values: x,y,z
    double x, y, z;

    // Task 2: Implement relevant constructors, destructors and assignment operators
    // -- constructors and destructors
    vec(double x, double y, double z) : x(x), y(y), z(z) {}     // parameterized ctor
    vec() : vec(0, 0, 0) {}                                     // default ctor
    vec(const vec&) = default;                                  // copy ctor
    vec(vec&&) = default;                                       // move ctor
    ~vec() = default;                                           // dtor
    // -- assignment operators
    vec& operator=(const vec&) = default;                       // copy assignment operator
    vec& operator=(vec&&) = default;                            // move assignment

    // Task 3: Implement arithmetic member operators (+=, -=, *=, /=)
    vec& operator+=(const vec&);
    vec& operator-=(const vec&);
    vec& operator*=(double);
    vec& operator/=(double);

    // Hint 3: Implement methods for vector operations
    double dot(const vec&) const;                               // dot-product
    vec cross(const vec&) const;                                // vector-product
    double norm() const;                                        // norm

    // Optional: Utility function to sets all components of vec at once
    void set(double a, double b, double c) { x = a; y = b; z = c; }

    // Task 4: Implement a print method utility for debugging
    void print(const std::string& s = "") const;                // for debugging

    // Task 5: Overload operator "operator<<" to send vec to stream output
    friend std::ostream& operator<<(std::ostream&, const vec&);
};

// Task 3: Implement arithmetic non-member operators (+, -, unary -, *, /)
vec operator+(vec, const vec&);
vec operator-(vec, const vec&);
vec operator-(const vec&);
vec operator*(vec, double);
vec operator*(double, vec);
vec operator/(vec, double);

// Hint 4: Make an approx method to compare two vec's with absolute precision "acc" and relative precision "eps"
bool approx(const vec&, const vec&, double acc = 1e-6, double eps = 1e-6);

// end