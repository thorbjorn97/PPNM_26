// integrator_stratified.h
#pragma once

#include <functional>
#include "vector.h" // Assuming this contains your pp::vector type

namespace pp {

struct stratified_result {
    double integral;
    double error;
};

// Stratified sampling multi-dimensional integrator
stratified_result stratified_mc(
    const std::function<double(const vector&)>& f,
    const vector& a,
    const vector& b,
    int N,
    int nmin,
    std::function<double()> rng
);

} // namespace pp