// "vec.cc" source file: Implementation file (cf. Hint 1)
#include <iostream>
#include <string>
#include <cmath>
#include "vec.h" // To implement the things declared in the vec.h file (cf. Hint 1)

// Task 4: Implement a simple print method for debugging
void vec::print(const std::string& s) const{
	std::cout << s << x << " " << y << " " << z << std::endl;
} // (using Hint 2)

// Task 3: Implement aritmetic member operators (+=, -=, *=, /=)
vec& vec::operator+=(const vec& v){
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
vec& vec::operator-=(const vec& v){
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
vec& vec::operator*=(double c){
    x *= c;
    y *= c;
    z *= c;
    return *this;
}
vec& vec::operator/=(double c){
    if(c == 0) throw std::runtime_error("Division by zero");
    x /= c;
    y /= c;
    z /= c;
    return *this;
}

// Task 3: Implement arithmetic non-member operators (+, -, unary -, *, /)
vec operator+(vec a, const vec& b){
    a += b;
    return a;
} // Task 6: Modern form of "operator+" is implemented.
// Task 6: The difference in the semantics between the traditional and modern form is the following: In the traditional form, a copy of the first operand is created inside the function. In the modern form, the first operand is passed by value, so the copy is made automatically at the call site and modified directly. This avoids the explicit temporary variable and allows better optimization through move semantics and copy elision.
vec operator-(vec a, const vec& b){
    a -= b;
    return a;
}
vec operator-(const vec& v){
    return vec(-v.x, -v.y, -v.z);
}
vec operator*(vec v, double c){
    v *= c;
    return v;
}
vec operator*(double c, vec v){
    v *= c;
    return v;
}
vec operator/(vec v, double c){
    v /= c;
    return v;
}

// Task 5: Overload operator "operator<<" to send vec to stream output
std::ostream& operator<<(std::ostream& os, const vec& v){
	os << "{ " << v.x << ", " << v.y << ", " << v.z << " } ";
	return os;
} // (using Hint 5)

// Hint 4: Make an "approx" method to compare two vec's with absolute precision "acc" and relative precision "eps"
// -- scalar comparison (helper function)
bool approx(double a, double b,
            double acc,
            double eps)
{
    if(std::abs(a-b) <= acc) return true;
    if(std::abs(a-b) <= eps*(std::abs(a)+std::abs(b))) return true;
    return false;
}
// -- vector comparison
bool approx(const vec& a, const vec& b,
            double acc,
            double eps)
{
    if(!approx(a.x,b.x,acc,eps)) return false;
    if(!approx(a.y,b.y,acc,eps)) return false;
    if(!approx(a.z,b.z,acc,eps)) return false;
    return true;
} //

// Hint 3: Implement methods for vector operators (dot-product, vector-product and norm)
double vec::dot(const vec& v) const{
    return x*v.x + y*v.y + z*v.z;
}
vec vec::cross(const vec& v) const{
    return vec(
        y*v.z - z*v.y,
        z*v.x - x*v.z,
        x*v.y - y*v.x
    );
}
double vec::norm() const{
    return std::sqrt(dot(*this));
}

// end