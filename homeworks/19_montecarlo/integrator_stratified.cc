// integrator_stratified.cc
// Heavy use of Gemini

#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

#include "integrator_stratified.h"

namespace pp {

// Helper function to run a quick local Plain MC to get estimate and variance
static void local_plainmc(
    const std::function<double(const vector&)>& f,
    const vector& a,
    const vector& b,
    int N,
    std::function<double()> rng,
    double& out_integral,
    double& out_variance,
    double V
) {
    if (N <= 0) {
        out_integral = 0.0;
        out_variance = 0.0;
        return;
    }

    size_t dim = a.size();
    double sum = 0.0;
    double sum2 = 0.0;
    vector x(dim);
    int valid_points = 0;

    for (int i = 0; i < N; ++i) {
        for (size_t d = 0; d < dim; ++d) {
            x[d] = a[d] + rng() * (b[d] - a[d]);
        }
        double fx = f(x);

        // SAFEGUARD 1: Filter out singularities (inf / nan values)
        if (!std::isfinite(fx)) {
            continue; 
        }

        sum += fx;
        sum2 += fx * fx;
        valid_points++;
    }

    // If all points hit a singularity, fallback gracefully
    if (valid_points == 0) {
        out_integral = 0.0;
        out_variance = 0.0;
        return;
    }

    double mean = sum / static_cast<double>(valid_points);
    out_integral = mean * V;
    
    // Variance calculation: <f^2> - <f>^2
    out_variance = (sum2 / static_cast<double>(valid_points)) - (mean * mean);
    
    // Guard against precision underflow or invalid numbers
    if (out_variance < 0.0 || !std::isfinite(out_variance)) {
        out_variance = 0.0; 
    }
}

stratified_result stratified_mc(
    const std::function<double(const vector&)>& f,
    const vector& a,
    const vector& b,
    int N,
    int nmin,
    std::function<double()> rng
) {
    size_t dim = a.size();
    
    // Calculate total volume V
    double V = 1.0;
    for (size_t d = 0; d < dim; ++d) {
        V *= (b[d] - a[d]);
    }

    // Base Case: If N is too small or less than nmin, run plain MC immediately
    if (N < nmin || N <= 4) {
        double integral = 0.0;
        double variance = 0.0;
        local_plainmc(f, a, b, N, rng, integral, variance, V);
        
        stratified_result res;
        res.integral = integral;
        res.error = V * std::sqrt(variance / static_cast<double>(N));
        if (!std::isfinite(res.error)) res.error = 0.0;
        return res;
    }

    // Step 1: Sample nmin points and estimate the baseline volume metrics
    double init_integral = 0.0;
    double init_variance = 0.0;
    local_plainmc(f, a, b, nmin, rng, init_integral, init_variance, V);

    // Step 2: Find the dimension with the largest sub-variance volatility
    size_t kdiv = 0;
    double max_subvariance_diff = -1.0;
    
    std::vector<double> var_left(dim, 0.0);
    std::vector<double> var_right(dim, 0.0);

    // Evaluate sub-volumes along each dimension 
    for (size_t d = 0; d < dim; ++d) {
        double mid = (a[d] + b[d]) / 2.0;
        
        vector a_sub = a;
        vector b_sub = b;
        
        // Left sub-volume analysis
        b_sub[d] = mid;
        double V_sub = V / 2.0;
        double dummy_int = 0.0;
        local_plainmc(f, a, b_sub, nmin / 2, rng, dummy_int, var_left[d], V_sub);
        
        // Right sub-volume analysis
        a_sub[d] = mid;
        b_sub[d] = b[d];
        local_plainmc(f, a_sub, b, nmin / 2, rng, dummy_int, var_right[d], V_sub);

        double total_dim_var = var_left[d] + var_right[d];
        if (std::isfinite(total_dim_var) && total_dim_var > max_subvariance_diff) {
            max_subvariance_diff = total_dim_var;
            kdiv = d;
        }
    }

    // Step 3: Subdivide the volume along our chosen dimension
    vector b_left = b;
    b_left[kdiv] = (a[kdiv] + b[kdiv]) / 2.0;

    vector a_right = a;
    a_right[kdiv] = (a[kdiv] + b[kdiv]) / 2.0;

    // Point allocation balance
    int N_rem = N - nmin;
    if (N_rem < 4) {
        // If not enough remaining points exist, split evenly
        int N_half = N_rem / 2;
        auto res_left  = stratified_mc(f, a, b_left, N_half, nmin, rng);
        auto res_right = stratified_mc(f, a_right, b, N_rem - N_half, nmin, rng);
        
        stratified_result grand_result;
        grand_result.integral = res_left.integral + res_right.integral;
        grand_result.error = std::hypot(res_left.error, res_right.error);
        return grand_result;
    }

    double sum_var = var_left[kdiv] + var_right[kdiv];
    int N_left = N_rem / 2;
    int N_right = N_rem / 2;

    // Divide remaining points proportional to sub-variances safely
    if (std::isfinite(sum_var) && sum_var > 1e-11) {
        double ratio = var_left[kdiv] / sum_var;
        // SAFEGUARD 2: Bound the ratio between 5% and 95% to avoid starving one half
        if (ratio < 0.05) ratio = 0.05;
        if (ratio > 0.95) ratio = 0.95;

        N_left = static_cast<int>(static_cast<double>(N_rem) * ratio);
        N_right = N_rem - N_left;
    }

    // SAFEGUARD 3: Ensure strict lower boundary constraints per sub-branch call
    if (N_left < 2) { N_left = 2; N_right = N_rem - 2; }
    if (N_right < 2) { N_right = 2; N_left = N_rem - 2; }

    // Step 4: Dispatch two recursive calls on the clean sub-volumes
    stratified_result res_left  = stratified_mc(f, a, b_left, N_left, nmin, rng);
    stratified_result res_right = stratified_mc(f, a_right, b, N_right, nmin, rng);

    // Step 5: Estimate grand outcomes
    stratified_result grand_result;
    grand_result.integral = res_left.integral + res_right.integral;
    
    // Using std::hypot handles error calculation cleanly without manual overflow risk
    grand_result.error = std::hypot(res_left.error, res_right.error);

    return grand_result;
}

} // namespace pp
