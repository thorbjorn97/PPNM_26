// "vec.h" header file: Interface for the class vec (cf. Hint 1)

// {new} NOTE: All implementations are in this header file as this is a template class.
// {new} Templates must be fully defined in headers (no separate .cc file).

#pragma once
#include <iostream>
#include <string>
#include <cmath> // {new: for std::abs and std::sqrt}
#include <stdexcept> // {new: for std::runtime_error} 

template<typename T> // {new: added template parameter T}
// {new} T is the scalar type of the vector components (e.g. float or double)

// Task 1: Define a class vec
struct vec {
    // {mod} Task 1: Let each instance of vec hold three components: x,y,z
    // {new} Instead of double, the components are now of type T, enabling vec<float>, vec<double>, etc.
    T x, y, z;

    // Task 2: Implement relevant constructors, destructors and assignment operators
    // -- constructors and destructors
    vec(T x, T y, T z) : x(x), y(y), z(z) {}                    // {mod} parameterized ctor (generalized to T)
    vec() : vec(T(0), T(0), T(0)) {}                            // {mod} default ctor (delegating to parameterized ctor with T(0))
    vec(const vec&) = default;                                  // copy ctor
    vec(vec&&) = default;                                       // move ctor
    ~vec() = default;                                           // dtor
    // -- assignment operators
    vec& operator=(const vec&) = default;                       // copy assignment operator
    vec& operator=(vec&&) = default;                            // move assignment operator

    // Task 3: Implement arithmetic member operators (+=, -=, *=, /=)
    // {new} Component-wise operations are implemented and *this returned to allow chaining
    vec& operator+=(const vec& v) {x += v.x; y += v.y; z += v.z; return *this;} // {mod: generalized to T}
    vec& operator-=(const vec& v) {x -= v.x; y -= v.y; z -= v.z; return *this;} // {mod: generalized to T}
    vec& operator*=(T c) {x *= c; y *= c; z *= c; return *this;} // {mod: generalized to T}
    vec& operator/=(T c) {
        if (c == T(0)) throw std::runtime_error("Division by zero");
        x /= c; y /= c; z /= c;
        return *this;
    } // {mod: added safety check and generalized to T}

    // Hint 3: Implement methods for vector operations
    // {new} Component-wise operations are implemented using mathematical definitions
    T dot(const vec& v) const {return x*v.x + y*v.y + z*v.z;} // {mod: dot-product, generalized to T}
    vec cross(const vec& v) const {
        return vec(
            y*v.z - z*v.y,
            z*v.x - x*v.z,
            x*v.y - y*v.x
        );                                       
    } // {mod: vector-product, generalized to T}
    T norm() const {return std::sqrt(dot(*this));} // {mod: norm, generalized to T}

    // Optional: Utility function to set all components of vec at once
    void set(T a, T b, T c) { x = a; y = b; z = c; } // {mod: generalized to T}

    // Task 4: Implement a print method utility for debugging
    void print(const std::string& s = "") const{
        std::cout << s << x << " " << y << " " << z << std::endl;
    } // {mod: definition moved from .cc file into class body for template compatibility}

    // Task 5: Overload operator "operator<<" to send vec to stream output
    friend std::ostream& operator<<(std::ostream& os, const vec& v) {
        os << "{ " << v.x << ", " << v.y << ", " << v.z << " } ";
        return os;
    } // {mod: definition moved from .cc file into class body for template compatibility}
};

// Task 3: Implement arithmetic non-member operators (+, -, unary -, *, /)
// {new} Each operator is implemented using pass-by-value with its own template
template<typename T> // {new: template for operator+}
vec<T> operator+(vec<T> a, const vec<T>& b) {a += b; return a; } // {mod}

template<typename T> // {new: template for operator-}
vec<T> operator-(vec<T> a, const vec<T>& b) {a -= b; return a; } // {mod}

template<typename T> // {new: template for unary -}
vec<T> operator-(const vec<T>& v) { return vec<T>(-v.x, -v.y, -v.z); } // {mod} (unary -)

template<typename T> // {new: template for operator* with scalar on right}
vec<T> operator*(vec<T> v, T c) {v *= c; return v;} // {mod}

template<typename T> // {new: template for operator* with scalar on left}
vec<T> operator*(T c, vec<T> v) {v *= c; return v;} // {mod}

template<typename T> // {new: template for operator/}
vec<T> operator/(vec<T> v, T c) {v /= c; return v;} // {mod}

// Hint 4: Make an approx method to compare two vec's with absolute precision "acc" and relative precision "eps"
template<typename T> // {new: template for approx method on scalars}
bool approx(T a, T b, T acc = 1e-6, T eps = 1e-6) {
    // {new} Two-stage comparison:
    // {new} 1. Absolute precision: |a - b| <= acc
    if (std::abs(a - b) <= acc) return true;
    // {new} 2. Relative precision: |a - b| <= eps * (|a| + |b|)
    if (std::abs(a - b) <= eps * (std::abs(a) + std::abs(b))) return true;
    return false;
} // {new: approx method on SCALARS via composition of absolute and relative approx}

template<typename T> // {new: template for approx method on vectors}
bool approx(const vec<T>& a, const vec<T>& b, T acc = 1e-6, T eps = 1e-6) {
    return approx(a.x, b.x, acc, eps) &&
           approx(a.y, b.y, acc, eps) &&
           approx(a.z, b.z, acc, eps);
} // {new: approx method on VECTORS via composition of scalar approx}

// end
