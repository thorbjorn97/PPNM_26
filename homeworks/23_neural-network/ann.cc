// ann.cc

#include <cmath>
#include <cmath>

#include "ann.h"
#include "minimizer.h"

namespace pp {

// Constructor for the ANN
ann::ann(int num_neurons) : n(num_neurons) {
    p = vector(3 * n);  // allocate parameter array for 3 parameters per neuron (will become p={ai,bi,wi}i=1..n)
     
    // Define activation function and its analytical gradient
    f = [](double u) { return u*std::exp(-u*u); };  // activation function (here: Gaussian wavelet, as suggested by instruction)
    df = [](double u) { return (1.0-2.0*u*u)*std::exp(-u*u); };  // analytical gradient
}

// Compute return value (network response y for a given input signal x received from the input neuron)
double ann::response(double x) const {
    double sum = 0.0;               // initialize response of summation neuron
    for (int i = 0; i < n; ++i) {   // for each hidden neuron i
        double a = p[3 * i + 0];      // set position shift  (shifts center of ripple)
        double b = p[3 * i + 1];      // set width scale     (scales width of ripple)
        double w = p[3 * i + 2];      // set weight factor   (scales height of ripple)
        sum += f((x - a) / b) * w;    // add contribution (output signal y_i) from hidden neuron i 
    }
    return sum;  // return network response y = F_p(x) = ∑i f((x-ai)/bi)*wi
}

// Train the network to interpolate the given table {x,y}
void ann::train(const vector& x_data, const vector& y_data) {
    size_t N = x_data.size();

    // Define the cost function expected by the minimizer framework
    auto cost_function = [this, &x_data, &y_data, N](const vector& trial_p) -> double {
        // Temporarily assign trial parameters to compute network response
        this->p = trial_p;                                         // overwrite this instance's p vector
        double sum_sq_errors = 0.0;                                // initialize sum of squared errors
        for (size_t k = 0; k < N; ++k) {                           // for each tabulated training data point (xk, yk)
            double error = this->response(x_data[k]) - y_data[k];  // compute deviation (F_p(xk) - yk)
            sum_sq_errors += error * error;                        // accumulate squared deviations: ∑k=1..N (Fp(xk) - yk)²
        }
        return sum_sq_errors;                                      // return cost function value: C(p) = ∑k=1..N (Fp(xk) - yk)²
    };

    // Initialize parameter vector p so each neuron starts on a different section
    for (int i = 0; i < n; ++i) {                          // for each hidden neuron
        p[3 * i + 0] = -1.0 + 2.0 * i / (n > 1 ? n - 1 : 1); // distribute ripple centers uniformly across [-1, 1]
        p[3 * i + 1] = 0.8;                                  // initial width scale b
        p[3 * i + 2] = 1.0;                                  // initial weight factor w
    }

    // Invoke Newton minimizer from minimizer.h
    double accuracy = 1e-5;     // convergence accuracy
    int max_iterations = 2000;  // maximum number of iterations

    // Execute minimizer on cost function to find optimized parameters p
    minimizer_result res = minimize_newton(cost_function, p, accuracy, max_iterations, "ann_train_log.txt");
    
    // Save final optimized parameters back to class instance
    p = res.min_point;        // store optimized internal parameters: p={ai,bi,wi}i=1..n
    steps = res.steps_taken;  // store number of steps taken by minimizer

}


// [TASK B] Derivative response function
double ann::response_derivative(double x) const {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        double a = p[3 * i + 0];
        double b = p[3 * i + 1];
        double w = p[3 * i + 2];
        
        double u = (x - a) / b;
        double df_du = (1.0 - 2.0 * u * u) * std::exp(-u * u);
        sum += (w / b) * df_du; // 1/b from chain rule
    }
    return sum;
}

// [TASK B] Second derivative response function
double ann::response_second_derivative(double x) const {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        double a = p[3 * i + 0];
        double b = p[3 * i + 1];
        double w = p[3 * i + 2];
        
        double u = (x - a) / b;
        double d2f_du2 = (4.0 * u * u * u - 6.0 * u) * std::exp(-u * u);
        sum += (w / (b * b)) * d2f_du2; // 1/b^2 from chain rule
    }
    return sum;
}

// [TASK B] Anti-derivative response function
double ann::response_integral(double x) const {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        double a = p[3 * i + 0];
        double b = p[3 * i + 1];
        double w = p[3 * i + 2];
        
        double u = (x - a) / b;
        double integral_f = -0.5 * std::exp(-u * u);
        sum += w * b * integral_f; // multiplication by b from dx = b*du
    }
    return sum;
}


} // namespace pp

// Written with the help of AI (Gemini 3.5 Flash)
