// integrator_mc.h

#pragma once

#include <functional>

#include "vector.h"

namespace pp {

struct mc_result {
    double integral;
    double error;
};

struct mc_point {
    int N;
    double integral;
    double error;
};

struct mc_stream_result {
    std::vector<mc_point> history;
};

mc_result plain_mc(
    const std::function<double(const vector&)>& f,
    const vector& a,
    const vector& b,
    int N,
    std::function<double()> rng
);

}