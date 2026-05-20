// lsfit.cc

#include "lsfit.h"
#include "qr.h"

namespace pp {

// ========================================================
// Constructs matrix A and target vector b, scaling both by 
// uncertainties (preparation for Weighted Least Squares).
// ========================================================
static void build_system(
    const std::vector<std::function<double(double)>>& fs,
    const vector& x,
    const vector& y,
    const vector& dy,
    matrix& A,
    vector& b
){
    size_t n = x.size();  // number of data points
    size_t m = fs.size();  // number of functions

    for (size_t i = 0; i < n; i++) {  // loop over data points

        // Map uncertainty-weighted y[i] to b
        b[i] = y[i] / dy[i];

        for (size_t k = 0; k < m; k++) { // loop over functions
            // Map uncertainty-weighted fs[k] to A[col=k][row=i]
            A[k][i] = fs[k](x[i]) / dy[i];
        }
    }
}

// ========================================================
// Solves upper triangular system Rx = b via back substitution.
// Expects R[i][j] = R[row][col]. Valid for i <= j.
// ========================================================
static vector solve_upper(const matrix& R, const vector& b)
{
    size_t m = b.size();  // number of equations in R
    vector x(m);  // define solution vector: x[row]
    
    // Loop over equations (last to first row)
    for (int i = (int)m - 1; i >= 0; i--) {
        
        double sum = 0.0; // initialize sum

        // Accumulate already computed components
        for (size_t j = i + 1; j < m; j++) {
            sum += R[i][j] * x[j];
        }

        // Solve for current term
        x[i] = (b[i] - sum) / R[i][i];
    }
    return x;
}

// ========================================================
// Inverts upper triangular matrix R by solving RX = I
// (solves R*x_j = e_j for each basis vector e_j).
// ========================================================
static matrix invert_R(const matrix& R)
{
    size_t m = R.cols_count();  // number of equations in R
    matrix Rinv(m, m);  // initialize inverse of R

    for (size_t j = 0; j < m; j++) { // loop over columns in R
        
        // Set j-th column of I as target vector e_j
        vector e(m);
        e[j] = 1.0;                       

        // Solve R*x_j = e_j using back substitution
        vector col = solve_upper(R, e);

        // Store solution as j-th column of Rinv
        Rinv[j] = col;
    }

    return Rinv;
}

// ========================================================
// TASK A: Weighted Least Squares (coefficients only).
// ========================================================
vector lsfit(
    const std::vector<std::function<double(double)>>& fs,
    const vector& x,
    const vector& y,
    const vector& dy)
{
    size_t n = x.size();  // number of data points
    size_t m = fs.size();  // number of functions

    matrix A(n, m);  // initialize system matrix: A[col][row]
    vector b(n);  // initialize target vector: b[row]

    // Construct A and b
    build_system(fs, x, y, dy, A, b);

    // Decompose A into an orthogonal Q and an upper triangular R
    qr decomp(A);

    // Solve R * c = Q^T * b and return parameter vector c
    vector c = decomp.solve(b);
    
    return c;
}

// ========================================================
// TASK B: Weighted least squares (coefficients and covariance).
// With QR-decomposition, Cov = R^{-1} * (R^{-1})^T.
// ========================================================

FitResult lsfit_mod(
    const std::vector<std::function<double(double)>>& fs,
    const vector& x,
    const vector& y,
    const vector& dy)
{
    size_t n = x.size();  // number of data points
    size_t m = fs.size();  // number of functions

    matrix A(n, m);  // initialize system matrix: A[col][row]
    vector b(n);  // initialize target vector: b[row]

    // Construct A and b
    build_system(fs, x, y, dy, A, b);

    // Decompose A into an orthogonal Q and an upper triangular R
    qr decomp(A);

    // Create output 1: Coefficient vector c
    vector c = decomp.solve(b); // solve R * c = Q^T * b and return c

    // Create output 2: Covariance matrix cov
    matrix R = decomp.getR();  // Get R from QR
    matrix Rinv = invert_R(R);  // invert R

    matrix cov(m, m);  // initialize covariance matrix: cov[col][row]

    // Compute covariance matrix via multiplication Cov = R^{-1} * (R^{-1})^T
    for (size_t i = 0; i < m; i++) {  // i loops over rows in cov
        for (size_t j = 0; j < m; j++) {  // j loops over cols in cov

            double sum = 0.0; // initialize sum

            // Add terms to sum
            for (size_t k = 0; k < m; k++) { // k loops over cols in Rinv
                sum += Rinv[k][i] * Rinv[k][j];
                // Note: Rinv[k][i] * Rinv[k][j] = (Rinv)_{i,k} * (Rinv^T)_{k,j}
            } 

            cov[i][j] = sum; // sum = cov[i][j] = Σ_{k=0...m-1} Rinv[k][i] * Rinv[k][j]
        }
    }

    return {c, cov}; // return outputs: Coefficient vector and covariance matrix
}

} // namespace pp