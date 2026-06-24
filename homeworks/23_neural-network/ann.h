// ann.h

#pragma once

#include <functional>

#include "vector.h"

namespace pp {

struct ann {
    int n;                             // number of hidden neurons
    std::function<double(double)> f;   // activation function f(u)
    std::function<double(double)> df;  // ADDED: analytical derivative f'(u) (per instruction hint)
    vector p;                          // network parameters (flat array of size 3 * n)
    int steps;                         // ADDED: number of Newton iterations taken during training
    
    ann(int num_neurons);              // constructor
    double response(double x) const;   // return the response of the network to the input signal x
    void train(const vector& x_data, const vector& y_data);  // train the network to interpolate the given table {x,y}
    
    // For Task B
    double response_derivative(double x) const;         // Derivative
    double response_second_derivative(double x) const;  // Second Derivative
    double response_integral(double x) const;           // Anti-derivative

};

} // namespace pp

// Written with the help of AI (Gemini 3.5 Flash)
