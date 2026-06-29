// broyden.cc
// Based on code from minimizer.cc in homework 21_minimum
// Additions to this homework are marked with {new}

#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>

#include "broyden.h"
#include "linalg_utils.h"

namespace pp {

// {new} Global configuration string to set path tracking destination files
std::string broyden_log_filename = "";

// {new} Extractable Metrics State
int broyden_steps = 0;
double broyden_final_z = 0.0;
double broyden_final_gnorm = 0.0;
std::string broyden_exit_reason = "";

// {new} Logging function
static void log_step(std::ofstream& out, const vector& x) {
    if (!out.is_open()) return;
    for (size_t i = 0; i < x.size(); ++i) {
        out << x[i] << (i + 1 == x.size() ? "" : " ");
    }
    out << "\n";
}

// Numerical gradient calculation
vector gradient(const std::function<double(const vector&)>& f, const vector& x, double f_x) {
    size_t n = x.size();
    vector x_mod = x;
    vector g(n);

    for (size_t i = 0; i < n; ++i) {
        double dxi = (1.0 + std::abs(x_mod[i])) * std::pow(2.0, -26);
        x_mod[i] += dxi;
        g[i] = (f(x_mod) - f_x) / dxi;
        x_mod[i] -= dxi; // Restore state
    }
    return g;
}

// Numerical Hessian calculation
matrix hessian(const std::function<double(const vector&)>& f, const vector& x, const vector& g_x) {
    size_t n = x.size();
    matrix H(n, n);
    vector x_mod = x;

    for (size_t j = 0; j < n; ++j) {
        double dxj = (1.0 + std::abs(x_mod[j])) * std::pow(2.0, -13);
        x_mod[j] += dxj;

        // Optimization: Pass the forward perturbed point evaluation directly to the inner gradient
        double f_perturbed = f(x_mod);
        vector g_xdx = gradient(f, x_mod, f_perturbed);

        for (size_t i = 0; i < n; ++i) {
            H(i, j) = (g_xdx[i] - g_x[i]) / dxj; // Access via row i, column j
        }
        x_mod[j] -= dxj; // Restore state
    }
    return H;
}

// =============================================================================
// STANDARD BROYDEN IMPLEMENTATION WITH ABSOLUTE SAFEGUARDS
// =============================================================================
// {new} Quasi-Newton algorithm using the Standard Broyden's update
vector broyden(std::function<double(const vector&)> f, vector x, double acc) {
    size_t n = x.size();  // Number of dimensions
    matrix B = matrix::identity(n); // Set inverse Hessian to unity
    // I compute the inverse Hessian instead of the Hessian itself because it is more efficient (I avoid solving th linear equation at each iteration)
    // (cf p. 104: "In practice one typically uses the inverse Hessian matrix... thus avoiding the need to solve the linear equation at each iteration")
    
    // Reset global tracking states for this run
    broyden_steps = 0;
    broyden_final_z = 0.0;
    broyden_final_gnorm = 0.0;
    broyden_exit_reason = "Max iterations exceeded or forced break"; // Default fallback

    std::ofstream log_stream;
    if (!broyden_log_filename.empty()) {
        log_stream.open(broyden_log_filename); 
    }

    double f_x = f(x);
    vector g = gradient(f, x, f_x); // Pass pre-evaluated f_x
    
    // Pass the active stream handle
    log_step(log_stream, x);

    // Protect against infinite loop with a safety guard
    const int max_steps = 10000;

    while (g.norm() > acc && broyden_steps < max_steps) { // loop until accuracy is met (so broyden will never return if acc < 0)
        broyden_steps++; // Increment step counter at the beginning of each loop

        // Calculate Newton's step delta_x = -B * ∇ϕ using matmul
        matrix G(n, 1);
        G[0] = g;
        matrix BG = matmul(B, G);
        vector dx = BG[0] * -1.0;

        // If not a descent direction, reset B = I and take steepest descent step
        if (g.dot(dx) >= 0.0) {
            B = matrix::identity(n);
            dx = g * -1.0;
        }

        double lambda = 1.0;               // Initial step size
        double lambda_min = 1.0 / 1024.0;  // Minimum step size
        bool hit_lambda_min = false;

        // Backtracking Line Search Loop
        while (true) {
            vector x_trial = x + (dx * lambda);  // Trial point
            double f_trial = f(x_trial);

            if (f_trial < f_x) {
                x = x_trial;
                f_x = f_trial;
                break;
            }

            if (lambda < lambda_min) {
                // Take step unconditionally per page 102 instructions
                x = x_trial;
                f_x = f_trial;
                hit_lambda_min = true;
                break;
            }
            lambda /= 2.0;
        }

        // Gather displacement data for secant updates
        vector g_next = gradient(f, x, f_x);
        vector dg = g_next - g;   // This is 'y' in the text
        vector s = dx * lambda;   // This is 's' in the text

        if (hit_lambda_min) {
            // Page 104 rule: Reset B to identity if line search bottoms out
            B = matrix::identity(n);
        } else {
            // Compute By = B * dg using matmul
            matrix DG(n, 1);
            DG[0] = dg;
            matrix BDG = matmul(B, DG);
            vector By = BDG[0];

            vector u = s - By;
            
            double sTy = s.dot(dg);
            double s_norm = s.norm();
            double y_norm = dg.norm();

            // Page 104 Safeguard against division by zero
            if (std::abs(sTy) > 1e-6 * s_norm * y_norm) {
                // Update B column-by-column, row-by-row matching your array syntax
                for (size_t j = 0; j < n; ++j) {
                    for (size_t i = 0; i < n; ++i) {
                        B[j][i] += (u[i] * s[j]) / sTy;
                    }
                }
            }
        }
    
        g = g_next;
        log_step(log_stream, x); // fast memory-buffered write
    }
    

    // Save final state properties out to the namespace state variables before returning
    broyden_final_z = f_x;
    broyden_final_gnorm = g.norm();
    
    if (std::isnan(broyden_final_gnorm) || std::isinf(broyden_final_z)) {
        broyden_exit_reason = "Divergence: NaN or Inf encountered";
    } else if (broyden_final_gnorm <= acc) {
        broyden_exit_reason = "Convergence achieved: ‖∇f(x)‖ < acc";
    } else if (broyden_steps >= max_steps) {
        broyden_exit_reason = "Maximum steps reached";
    } else {
        broyden_exit_reason = "Early termination / Internal loop break";
    }

    return x;
}

// {new} Quasi-Newton algorithm using the Symmetrized Broyden's update for comparison
vector broyden_symmetrized(std::function<double(const vector&)> f, vector x, double acc) {
    size_t n = x.size();
    matrix B = matrix::identity(n);
    
    // Reset global tracking states for this run
    broyden_steps = 0;
    broyden_final_z = 0.0;
    broyden_final_gnorm = 0.0;
    broyden_exit_reason = "Max iterations exceeded or forced break";

    // Open the stream ONCE in standard output mode (which defaults to truncation!)
    std::ofstream log_stream;
    if (!broyden_log_filename.empty()) {
        log_stream.open(broyden_log_filename); 
    }

    double f_x = f(x);
    vector g = gradient(f, x, f_x);
    
    // Pass the active stream handle
    log_step(log_stream, x);
    const int max_steps = 10000;

    while (g.norm() > acc && broyden_steps < max_steps) {
        broyden_steps++; // Increment step counter at the beginning of each loop

        // Calculate Newton's step delta_x = -B * ∇ϕ using matmul
        matrix G(n, 1);
        G[0] = g;
        matrix BG = matmul(B, G);
        vector dx = BG[0] * -1.0;

        if (g.dot(dx) >= 0.0) { // If dx is not a descent direction, reset B = I
            B = matrix::identity(n);
            dx = g * -1.0;
        }

        double lambda = 1.0;
        double lambda_min = 1.0 / 1024.0;
        bool hit_lambda_min = false;

        while (true) {
            vector x_trial = x + (dx * lambda);
            double f_trial = f(x_trial);

            if (f_trial < f_x) {
                x = x_trial;
                f_x = f_trial;
                break;
            }

            if (lambda < lambda_min) {
                x = x_trial;
                f_x = f_trial;
                hit_lambda_min = true;
                break;
            }
            lambda /= 2.0;
        }

        vector g_next = gradient(f, x, f_x);
        vector dg = g_next - g; 
        vector s = dx * lambda; 

        if (hit_lambda_min) {
            // Line search bottomed out on a ridge/saddle: Reset memory
            B = matrix::identity(n);
        } else {
            // Compute By = B * dg using matmul
            matrix DG(n, 1);
            DG[0] = dg;
            matrix BDG = matmul(B, DG);
            vector By = BDG[0];

            vector u = s - By;
            
            double sTy = s.dot(dg);
            double s_norm = s.norm();
            double y_norm = dg.norm();

            if (std::abs(sTy) > 1e-6 * s_norm * y_norm) {
                double uTy = u.dot(dg);
                double gamma = uTy / (2.0 * sTy);
                
                // Vector a = (u - gamma * s) / (s^T * y)
                vector a = (u - (s * gamma)) * (1.0 / sTy);

                // Apply rank-2 symmetric addition matching column-major storage layout
                for (size_t j = 0; j < n; ++j) {
                    for (size_t i = 0; i < n; ++i) {
                        B[j][i] += (a[i] * s[j]) + (s[i] * a[j]);
                    }
                }
            }
        }

        g = g_next;
        log_step(log_stream, x); // fast memory-buffered write
    }
    
    // Save final properties to namespace variables so the profiler can read them
    broyden_final_z = f_x;
    broyden_final_gnorm = g.norm();
    
    if (std::isnan(broyden_final_gnorm) || std::isinf(broyden_final_z)) {
        broyden_exit_reason = "Divergence: NaN or Inf encountered";
    } else if (broyden_final_gnorm <= acc) {
        broyden_exit_reason = "Convergence achieved: ‖∇f(x)‖ < acc";
    } else if (broyden_steps >= max_steps) {
        broyden_exit_reason = "Maximum steps reached";
    } else {
        broyden_exit_reason = "Early termination / Internal loop break";
    }

    return x;
}

} // namespace pp
