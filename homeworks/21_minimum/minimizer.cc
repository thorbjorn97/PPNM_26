// minimizer.cc

#include "minimizer.h"
#include "qr.h"
#include <cmath>
#include <algorithm>
#include <fstream>

namespace pp {

vector gradient(const std::function<double(const vector&)>& phi, const vector& x, double phi_x) {
    size_t n = x.size();
    vector x_mod = x;
    vector g(n);

    for (size_t i = 0; i < n; ++i) {
        double dxi = (1.0 + std::abs(x_mod[i])) * std::pow(2.0, -26);
        x_mod[i] += dxi;
        g[i] = (phi(x_mod) - phi_x) / dxi;
        x_mod[i] -= dxi; // Restore state
    }
    return g;
}

matrix hessian(const std::function<double(const vector&)>& phi, const vector& x, const vector& g_x) {
    size_t n = x.size();
    matrix H(n, n);
    vector x_mod = x;

    for (size_t j = 0; j < n; ++j) {
        double dxj = (1.0 + std::abs(x_mod[j])) * std::pow(2.0, -13);
        x_mod[j] += dxj;

        // Optimization: Pass the forward perturbed point evaluation directly to the inner gradient
        double phi_perturbed = phi(x_mod);
        vector g_xdx = gradient(phi, x_mod, phi_perturbed);

        for (size_t i = 0; i < n; ++i) {
            H(i, j) = (g_xdx[i] - g_x[i]) / dxj; // Access via row i, column j
        }
        x_mod[j] -= dxj; // Restore state
    }
    return H;
}

// [Task A] Newton's method with numerical gradient, numerical Hessian matrix and back-tracking linesearch
// Optimizations:
// - Receives the already-computed gradient vector to save (1 + n) evaluations
// - Uses adaptive regularization parameter to avoid getting stuck
// - Logs the trajectory of the minimizer to a file for visualization
minimizer_result minimize_newton(
    const std::function<double(const vector&)>& phi,
    const vector& x_start,
    double acc,
    int max_iter,
    const std::string& log_filename
) {
    vector x = x_start;
    int steps = 0;
    double mu = 1e-6;  // initialize adaptive regularization parameter
    double phi_x = phi(x);  // Optimization: evaluate phi(x) once at startup

    // Prepare for logging
    std::ofstream data_file;
    if (!log_filename.empty()) {
        data_file.open(log_filename);
        data_file << x[0] << " " << x[1] << "\n";
    }

    // Newton's method
    while (steps < max_iter) {                   // Newton iterations
        vector g = gradient(phi, x, phi_x);
        if (g.norm() < acc) break;               // job done

        // Optimization: Pass the already computed gradient 'g' straight to the Hessian
        matrix H_base = hessian(phi, x, g); 

        bool step_accepted = false;
        vector dx;
        double phi_trial = phi_x;
        double lambda = 1.0;

        // Inner adaptive loop: Adjust mu until a descent step direction is found
        while (!step_accepted) {
            matrix H = H_base;  // Optimization: pass the already computed Hessian matrix
            for (size_t i = 0; i < H.rows(); ++i) H(i, i) += mu;  // Levenberg regularization

            qr QR(H);
            dx = QR.solve(g * -1.0);

            lambda = 1.0;
            while (lambda >= 1.0 / 1024.0) {     // backtracking linesearch
                vector x_trial = x + (dx * lambda);
                phi_trial = phi(x_trial);
                
                if (phi_trial < phi_x) {         // good step
                    x = x_trial;
                    phi_x = phi_trial; // Optimization: cache the line-search value for the next loop iteration
                    step_accepted = true;
                    break;
                }
                lambda /= 2.0;
            }

            if (step_accepted) {
                // Step succeeded: gradually reduce mu to prefer pure Newton speed
                mu = std::max(mu / 10.0, 1e-6);
            } else {
                // Step failed (uphill direction): increase mu to force positive-definiteness
                mu = std::max(mu * 10.0, 1e-4);
                if (mu > 1e6) {
                    // Safety valve: prevent infinite loop if completely flat or saddle
                    x = x + (dx * lambda);
                    phi_x = phi(x);
                    break; 
                }
            }
        }

        if (data_file.is_open()) {
            data_file << x[0] << " " << x[1] << "\n";
        }

        steps++;
    }

    if (data_file.is_open()) data_file.close();
    return {x, steps};
}

} // namespace pp
