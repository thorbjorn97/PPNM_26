// rose_benchmark.h
#pragma once

#include <cmath>
#include "vector.h"

namespace pp {

// Responsible for benchmarking the N-dimensional Generalized Rosenbrock function in Task 6
struct RosenbrockBenchmarker {

    // N-Dimensional Generalized Rosenbrock function implementation
    static double rosenbrockNd(const vector& v, int& counter) {
        counter++;
        double sum = 0.0;
        for (size_t i = 0; i < v.size() - 1; ++i) {
            sum += 100.0 * std::pow(v[i+1] - v[i]*v[i], 2) + std::pow(1.0 - v[i], 2);
        }
        return sum;
    }

    // Computes Euclidean distance error vector component deviation
    static double calculateDeviation(const vector& v) {
        double deviation_sum = 0.0;
        for (size_t i = 0; i < v.size(); ++i) {
            deviation_sum += std::pow(v[i] - 1.0, 2);
        }
        return std::sqrt(deviation_sum);
    }
};

} // namespace pp
