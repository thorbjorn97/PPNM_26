// rootfinder.cc

#include <cmath>
#include <algorithm>
#include <iostream>

#include "rootfinder.h"
#include "matrix.h"
#include "qr.h"

namespace pp {

// [Task A] Helper function: Numerical estimation of Jacobian of f at x
matrix jacobian(
    const std::function<vector(const vector&)>& f,  // function to be analyzed
    const vector& x,                                // point at which Jacobian is to be evaluated 
    const vector& fx,                               // function value at x
    vector dx                                       // finite-difference scale (passed by value for easier modification)
) {

    vector dx_local = dx;  // initialize working copy of dx (avoid repeated allocation)
    size_t n = x.size();   // get dimension of the analyzed function

    // Set safe default dx in case dx is not provided or has incorrect size
    if (dx_local.size() != n) {
        dx_local = vector(n);  // initialize working copy of dx
        
        for (size_t i = 0; i < n; i++) {  // for each coordinate x_i in x, set default dx_i (as per instructions)
            dx_local[i] = std::max(std::abs(x[i]), 1.0) * std::pow(2.0, -26);
        }
    }

    matrix J(n,n);  // initialize Jacobian
    vector xh = x;  // initialize perturbed x as a copy of x (avoid repeated allocation and preserves x)

    for (size_t j = 0; j < n; j++) {  // for each coordinate j in x
        xh[j] += dx_local[j];         // perturb the j-th coordinate
        vector df = f(xh) - fx;       // perturb the function value (reuses fx to avoid recomputation of f(x))
        
        for (size_t i = 0; i < n; i++) {    // for each row i
            J[j][i] = df[i] / dx_local[j];  // compute j-th column of Jacobian (J[j][i] is J[col][row])
        }
        xh[j] = x[j];  // restore original x before next column perturbation
    }

    return J;  // return Jacobian

} // jacobian


// [Task A] Newton's method (simple back-tracking line-search)
newton_result newton(                               
    const std::function<vector(const vector&)>& f,  // function to be analyzed
    vector x,                                       // initial guess of root
    double acc,                                     // target accuracy
    double lambda_min,                              // minimum step size
    int max_iter,                                   // maximum number of iterations
    vector dx                                       // finite-difference scale (passed by value for easier modification)
) {
    
    size_t n = x.size();  // get dimension of the analyzed function
    
    // Set safe default dx in case dx is not provided or has incorrect size
    if (dx.size() != n) {
        dx = vector(n);
        for (size_t i = 0; i < n; i++) {
            dx[i] = std::max(std::abs(x[i]), 1.0) * std::pow(2.0, -26);
        }
    }

    vector fx = f(x);  // set function value at x
    int iterations = 0;  // initialize iteration counter

    // Set default exit reason
    std::string reason = "Max iterations reached (iter = max_iter = " + std::to_string(max_iter) + ")";

    // Main loop
    for (int iter = 0; iter < max_iter; iter++) {  // for each allowed iteration
        iterations = iter + 1;  // update iteration counter
        double fx_norm = fx.norm();  // compute ‖f(x)‖

        // Check target accuracy stopping condition
        if (fx_norm < acc) {
            reason = "Target accuracy reached (‖f(x)‖ < acc)";
            break;
        }

        // Compute Newton's step
        matrix J = jacobian(f, x, fx, dx);  // generate numerical Jacobian via helper function (above)
        qr QR(J);                           // compute QR decomposition of J using qr() from qr.h
        vector Dx = QR.solve(fx * -1.0);    // compute Newton's step

        // ========================================================================================
        // [Task A] Simple Back-tracking line-search
        // ========================================================================================
        double lambda = 1.0;  // initialize step-size
        vector z;  // initialize trial point
        vector fz;  // initialize function value at trial point
        while (true) {
            z = x + Dx * lambda;  // compute trial point
            fz = f(z);           // compute function value at trial point
            // Break out if function value at trial point is small enough or if lambda drops below limit
            if (fz.norm() < (1.0 - lambda / 2.0) * fx_norm || lambda <= lambda_min) {
                break;
            }
            lambda *= 0.5;  // halve the step-size if function value at trial point is too large
        }
        // ========================================================================================

        // Update x and fx before next iteration (and before testing for step-size exit condition)
        x = z; fx = fz;

        // Check step-size exit condition
        bool small_step = true;  // initialize small_step
        for (size_t i = 0; i < n; i++) {  // for each coordinate
            // Determine if actual step size ‖λ*Dx_i‖ is larger than ‖δx_i‖
            if (std::abs(lambda * Dx[i]) >= std::abs(dx[i])) {  // if true for any coordinate
                small_step = false;                            // set small_step to false (floor not reached)
                break;
            }
        }
        if (small_step) {  // Condition satisfied if ‖λ*Dx_i‖ < ‖δx_i‖ for all i
            reason = "Step-size floor reached (‖λ*Dx‖ < ‖δx‖)";
            break;
        }

    }

    // Final check for edge case convergence on the final step boundary
    if (fx.norm() < acc) {
        reason = "Target acc reached (‖f(x)‖ < acc)";
    }

    // Return solution
    return {x, reason, iterations};

} // newton


// [Task C] Newton's method (Broyden updates + Quadratic interpolation line-search)
newton_result newton_broyden_quadratic(
    std::function<vector(const vector&)> f,  // function to be analyzed
    vector x,                                // initial guess of root
    double acc,                              // target accuracy
    double lambda_min,                        // minimum step size
    int max_iter                             // maximum number of iterations
) {
    size_t n = x.size();  // get dimension of the analyzed function
    
    // Dynamic scale parameters for the initial finite difference step
    vector dx_local(n);
    for (size_t i = 0; i < n; i++) {
        dx_local[i] = std::max(std::abs(x[i]), 1.0) * std::pow(2.0, -26);
    }

    vector fx = f(x);      // set function value at x
    int iterations = 0;    // initialize iteration counter
    matrix J;              // initialize Jacobian J

    // Set default exit reason
    std::string reason = "Max iterations reached (iter = max_iter = " + std::to_string(max_iter) + ")";

    // Main loop
    for (int iter = 0; iter < max_iter; iter++) {
        iterations = iter + 1;       // update iteration counter
        double fx_norm = fx.norm();  // compute ‖f(x)‖
        
        // Check target accuracy stopping condition
        if (fx_norm < acc) {
            reason = "Target accuracy reached (‖f(x)‖ < acc)";  // update exit reason
            break; 
        }

        // Generate Numerical Jacobian via helper function on the very first step only
        if (iter == 0) {
            J = jacobian(f, x, fx, dx_local);
        }

        // Solve J * Dx = -fx
        qr QR(J);                         // compute QR decomposition of J using qr() from qr.h
        vector Dx = QR.solve(fx * -1.0);  // compute Newton's step
        
        // ========================================================================================
        // [Task C] Quadratic interpolation back-tracking line-search
        // ========================================================================================
        double lambda = 1.0;  // initialize step-size
        vector z = x + Dx * lambda;
        vector fz = f(z);
        
        // Define initial interpolation parameters from The Book
        double phi_0  = 0.5 * fx_norm * fx_norm;  // ϕ(0) = 1/2‖f(x)∥
        double dphi_0 = -fx_norm * fx_norm;       // ϕ'(0) = -‖f(x)∥

        // Loop until function value at trial point is small enough or if lambda drops below limit
        while (fz.norm() >= (1.0 - lambda / 2.0) * fx_norm && lambda >= lambda_min) {
            
            // Calculate quadratic model as ϕ(λ) = 1/2‖f(x)∥ from The Book
            double phi_lambda = 0.5 * fz.norm() * fz.norm();
            
            // Calculate quadratic coefficient c using Eq. (9.11) in The Book
            double c = (phi_lambda - phi_0 - dphi_0 * lambda) / (lambda * lambda);
            
            double lambda_next;
            if (c > 1e-10) {
                lambda_next = -dphi_0 / (2.0 * c);  // analytical minimum from Eq. (9.12)
            } else {
                lambda_next = lambda * 0.5;         // fallback step reduction
            }

            // Ensure lambda decreases but doesn't drop too aggressively
            lambda_next = std::max(0.1 * lambda, std::min(0.5 * lambda, lambda_next));
            
            lambda = lambda_next;
            z = x + Dx * lambda;
            fz = f(z);
        }

        // If line-search gets completely stuck, accept the step unconditionally 
        // to move away from the difficult location
        if (lambda < lambda_min) {
            lambda = lambda_min;
            z = x + Dx * lambda;
            fz = f(z);
        }

        // --- Broyden Rank-1 Algebraic Update ---
        vector s = z - x;    // step change in x
        vector y = fz - fx;  // change in function values
        
        double s_norm_sq = 0.0;
        for (size_t i = 0; i < n; i++) s_norm_sq += s[i] * s[i];

        // Only update Jacobian if the step taken was numerically significant
        if (s_norm_sq > 1e-20) {
            vector J_s(n);
            for (size_t i = 0; i < n; i++) {
                double sum = 0.0;
                for (size_t j = 0; j < n; j++) sum += J(i, j) * s[j];
                J_s[i] = sum;
            }
            vector r = y - J_s;  // residual error vector

            // Apply rank-1 correction to Jacobian matrix
            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < n; j++) {
                    J(i, j) += (r[i] * s[j]) / s_norm_sq;
                }
            }
        }

        // Update x and fx for the next iteration
        x = z; fx = fz;

        // Check step-size exit condition updating x, but only if not going to the floor fallback
        if (lambda > lambda_min) {
            bool small_step = true;
            for (size_t i = 0; i < n; i++) {
                if (std::abs(lambda * Dx[i]) >= std::abs(dx_local[i])) {
                    small_step = false;
                    break;
                }
            }
            if (small_step) {
                reason = "Step-size floor reached (‖λ*Dx‖ < ‖δx‖)";  // update exit reason
                break;
            }
        }
        // ========================================================================================

    }

    // Final check for edge case convergence on the final step boundary
    if (fx.norm() < acc) {
        reason = "Target acc reached (‖f(x)‖ < acc)";  // update exit reason
    }

    // Return solution
    return {x, reason, iterations};

} // newton_broyden_quadratic


} // namespace pp