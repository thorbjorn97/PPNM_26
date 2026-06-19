#include "integrator_qmc.h"
#include <cmath>

namespace pp {

mc_result quasi_mc(
    const std::function<double(const vector&)>& f,
    const vector& a,
    const vector& b,
    int N,
    halton& seq
) {
    size_t dim = a.size();

    double V = 1.0;
    for (size_t i = 0; i < dim; i++)
        V *= (b[i] - a[i]);

    double sum = 0.0;

    for (int n = 0; n < N; n++) {

        std::vector<double> u = seq.next();
        vector x(dim);

        for (size_t i = 0; i < dim; i++)
            x[i] = a[i] + u[i] * (b[i] - a[i]);

        sum += f(x);
    }

    double mean = sum / N;

    // NOTE: no statistically meaningful sigma for QMC
    return {mean * V, 0.0};
}

} // namespace pp