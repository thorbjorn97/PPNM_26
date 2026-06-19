// rng.h

#pragma once

#include <vector>
#include <random>

namespace pp {

// (LCG) Linear Congruential Generator ("quick-and-dirty")
class lcg {
private:
    uint32_t seed;     // seed (current state of the generator)
    const uint32_t a;  // multiplier
    const uint32_t c;  // increment
    static constexpr uint64_t m = 1ULL << 32; // modulus = 2^32
public:
    lcg(
        uint32_t s,                // initial seed
        uint32_t aa = 1664525,     // multiplier (as per "quick-and-dirty" generator)
        uint32_t cc = 1013904223   // increment (as per "quick-and-dirty" generator)
    )
        : seed(s), a(aa), c(cc) {}  // constructor

    double operator()() {

        // Advance the generator by one step: x_(n+1) = (a*x_n + c) mod 2^32
        // Since seed is a 32-bit unsigned integer, overflow
        // automatically performs the modulo 2^32 operation.
        seed = a*seed + c;

        // Convert the integer state to a floating-point number
        // in the interval [0,1), following the formula given in instructions
        return (seed + 1.0)/(m + 1.0);  // return a double in [0,1)
    }
};

// (STL) Standard C++ Library random number generator (here using Mersenne Twister)
class stl {
private:
    std::mt19937 engine;
    std::uniform_real_distribution<double> dist;
public:
    explicit stl(uint32_t s) 
        : engine(s), dist(0.0, 1.0) {}

    double operator()() {
        return dist(engine);
    }
};

// Halton sequence generator (low-discrepancy sequence)
// Produces points in [0,1)^d deterministically.
class halton {
private:
    std::vector<int> base;
    size_t index = 1;

    static double radical_inverse(size_t i, int b) {
        double f = 1.0;
        double result = 0.0;

        const size_t base_size_t = static_cast<size_t>(b);

        while(i > 0) {
            f /= static_cast<double>(b);
            result += f * static_cast<double>(i % base_size_t);
            i /= base_size_t;
        }

        return result;
    }
public:
    explicit halton(const std::vector<int>& bases)
        : base(bases) {}

    std::vector<double> next() {

        std::vector<double> x(base.size());

        for(size_t d = 0; d < base.size(); d++) {
            x[d] = radical_inverse(index, base[d]);
        }

        index++;

        return x;
    }
};

} // namespace pp