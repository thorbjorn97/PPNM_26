#pragma once

#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <vector>

namespace pp {

class vector {
    
private:
    std::vector<double> data;

public:
    vector() = default;

    explicit vector(size_t n) : data(n, 0.0) {}

    vector(std::initializer_list<double> init) : data(init) {}

    size_t size() const { return data.size(); }

    double& operator[](size_t i) { return data[i]; }
    const double& operator[](size_t i) const { return data[i]; }

    // vector addition
    vector operator+(const vector& other) const {
        if (data.size() != other.size()) throw std::runtime_error("Vector size mismatch");

        vector result(data.size());
        for (size_t i = 0; i < data.size(); i++) result[i] = data[i] + other[i];
        return result;
    }

    // vector subtraction
    vector operator-(const vector& other) const {
        if (data.size() != other.size()) throw std::runtime_error("Vector size mismatch");

        vector result(data.size());
        for (size_t i = 0; i < data.size(); i++) result[i] = data[i] - other[i];
        return result;
    }

    // scalar multiplication
    vector operator*(double scalar) const {
        vector result(data.size());
        for (size_t i = 0; i < data.size(); i++) result[i] = data[i] * scalar;
        return result;
    }

    // dot product
    double dot(const vector& other) const {
        if (data.size() != other.size()) throw std::runtime_error("Vector size mismatch");

        double sum = 0.0;
        for (size_t i = 0; i < data.size(); i++) sum += data[i] * other[i];
        return sum;
    }

    double norm() const { return std::sqrt(this->dot(*this)); }

}; // class vector

}  // namespace pp
