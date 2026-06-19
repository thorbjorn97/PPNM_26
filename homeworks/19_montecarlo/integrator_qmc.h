#pragma once

#include <functional>
#include <vector>

#include "vector.h"
#include "rng.h"
#include "integrator_mc.h"

namespace pp {

// Quasi-Monte Carlo integrator using Halton sequence
mc_result quasi_mc(
    const std::function<double(const vector&)>& f,
    const vector& a,
    const vector& b,
    int N,
    halton& seq
);

}