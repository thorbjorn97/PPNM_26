// linalg_utils.h

#pragma once

#include <cmath>
#include <random>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "matrix.h"
#include "vector.h"

namespace pp {

// ============================================================================
// New utility functions for this homework
// ============================================================================

// Random symmetric matrix generation for testing
inline matrix random_symmetric(int n)
{
    matrix A(n,n);

    std::mt19937 gen(44);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++){
            double x = dist(gen);
            A(i,j) = x;
            A(j,i) = x;
        }
    }

    return A;
}

// Frobenius norm of the difference between two matrices
inline double frob_diff(const matrix& A, const matrix& B)
{
    double sum = 0.0;

    for(size_t i = 0; i < A.rows(); i++)
    for(size_t j = 0; j < A.cols_count(); j++)
    {
        double d = A(i,j) - B(i,j);
        sum += d*d;
    }

    return std::sqrt(sum);
}

// Matrix-matrix multiplication: C = A B
matrix operator*(const matrix& A, const matrix& B)
{
    int n = A.rows();
    int m = A.cols_count();
    int p = B.cols_count();

    matrix C(n,p);

    for(int j = 0; j < p; j++)
        for(int i = 0; i < n; i++)
            for(int k = 0; k < m; k++)
                C(i,j) += A(i,k) * B(k,j);

    return C;
}

// Hamiltonian for radial hydrogen s-wave wavefunctions
matrix hydrogen_H(double rmax,double dr)
{
    int n=(int)(rmax/dr)-1;

    vector r(n);

    for(int i=0;i<n;i++)
        r[i]=dr*(i+1);

    matrix H(n,n);

    double coeff=-0.5/(dr*dr);

    for(int i=0;i<n-1;i++)
    {
        H(i,i)   =-2*coeff;

        H(i,i+1)=coeff;
        H(i+1,i)=coeff;
    }

    H(n-1,n-1)=-2*coeff;

    for(int i=0;i<n;i++)
        H(i,i)+=-1.0/r[i];

    return H;
}

// Sort helper
void sort_eigen(vector& e, matrix& V)
{
    int n = e.size();

    std::vector<std::pair<double,int>> idx;
    idx.reserve(n);

    for(int i=0;i<n;i++)
        idx.push_back({e[i], i});

    std::sort(idx.begin(), idx.end());

    vector e_sorted(n);
    matrix V_sorted(n,n);

    for(int k=0;k<n;k++)
    {
        e_sorted[k] = idx[k].first;

        for(int i=0;i<n;i++)
            V_sorted(i,k) = V(i, idx[k].second);
    }

    e = e_sorted;
    V = V_sorted;
}

// Ground state energy
double ground_state(double rmax, double dr)
{
    matrix H = hydrogen_H(rmax, dr);
    auto [e, V] = jacobi(H);

    sort_eigen(e, V);

    return e[0];
}

// ============================================================================
// Old utility functions from previous homeworks (unused are commented out)
// ============================================================================

// === Random vector and matrix generation ===

// Random vector generation for testing
// inline vector random_vector(size_t n) {

//     vector v(n);

//     std::mt19937 gen(42);
//     std::uniform_real_distribution<double> dist(-1.0, 1.0);

//     for (size_t i = 0; i < n; i++) {
//         v[i] = dist(gen);
//     }

//     return v;
// }

// Random matrix generation for testing
// inline matrix random_matrix(size_t n, size_t m) {
//     matrix A(n, m);

//     std::mt19937 gen(43);
//     std::uniform_real_distribution<double> dist(-1.0, 1.0);

//     for (size_t j = 0; j < m; j++) {
//         for (size_t i = 0; i < n; i++) {
//             A[j][i] = dist(gen);
//         }
//     }

//     return A;
// }

// === Matrix operations ===

// // Matrix-vector multiplication: y = M x
// inline vector matvec(const matrix& M, const vector& x) {

//     size_t n = M.rows();
//     size_t m = M.cols_count();

//     if (x.size() != m)
//         throw std::runtime_error("Dimension mismatch in matvec");

//     vector result(n);

//     for (size_t j = 0; j < m; j++) {
//         for (size_t i = 0; i < n; i++) {
//             result[i] += M[j][i] * x[j];
//         }
//     }

//     return result;
// }

// Matrix-matrix multiplication: C = A B
// inline matrix matmul(const matrix& A, const matrix& B) {

//     size_t n = A.rows();
//     size_t m = A.cols_count();
//     size_t p = B.cols_count();

//     if (m != B.rows()) {
//         throw std::runtime_error(
//             "Dimension mismatch in matmul"
//         );
//     }

//     matrix C(n, p);

//     for (size_t j = 0; j < p; j++) {

//         vector col(n);

//         for (size_t k = 0; k < m; k++) {
//             col = col + A[k] * B[j][k];
//         }

//         C[j] = col;
//     }

//     return C;
// }

// === Validation functions ===

// Check if R is upper triangular
// inline bool is_upper_triangular(const matrix& R) {
//     size_t m = R.cols_count();

//     for (size_t i = 1; i < m; i++) {
//         for (size_t j = 0; j < i; j++) {
//             if (std::abs(R[i][j]) > 1e-12)
//                 return false;
//         }
//     }
//     return true;
// }

// Check if Q is orthonormal (Q^T Q = I)
// inline bool check_orthonormal(const matrix& Q) {
//     size_t m = Q.cols_count();

//     for (size_t i = 0; i < m; i++) {
//         for (size_t j = 0; j < m; j++) {

//             double dot = Q[i].dot(Q[j]);

//             if (i == j) {
//                 if (std::abs(dot - 1.0) > 1e-12)
//                     return false;
//             } else {
//                 if (std::abs(dot) > 1e-12)
//                     return false;
//             }
//         }
//     }
//     return true;
// }

// Check if Q R reconstructs A within a tolerance
// inline bool check_reconstruction(const matrix& Q,
//                           const matrix& R,
//                           const matrix& A) {

//     size_t n = A.rows();
//     size_t m = A.cols_count();

//     matrix QR(n, m);

//     for (size_t j = 0; j < m; j++) {

//         vector col(n);

//         for (size_t k = 0; k < m; k++) {
//             col = col + Q[k] * R[k][j];
//         }

//         QR[j] = col;
//     }

//     for (size_t j = 0; j < m; j++) {
//         for (size_t i = 0; i < n; i++) {
//             if (std::abs(QR[j][i] - A[j][i]) > 1e-10)
//                 return false;
//         }
//     }

//     return true;
// }

// Check if a matrix is the identity
// inline bool check_identity(const matrix& M,
//                     double tol = 1e-10) {

//     size_t n = M.rows();

//     if (n != M.cols_count()) {
//         return false;
//     }

//     for (size_t j = 0; j < n; j++) {
//         for (size_t i = 0; i < n; i++) {

//             double expected = (i == j) ? 1.0 : 0.0;

//             if (std::abs(M[j][i] - expected) > tol) {
//                 return false;
//             }
//         }
//     }

//     return true;
// }

// === Print functions ===

// Print section header
inline void section(const std::string& title) {
    std::cout << "\n"
              << "===============================================================================\n"
              << title << "\n"
              << "===============================================================================\n";
}

// Print substep header
inline void substep(const std::string& title) {
    std::cout << "\n-- " << title << "\n";
}

// Print matrix with formatting
inline void print_matrix(const pp::matrix& M, const std::string& name = "") {

    if (!name.empty())
        std::cout << name << ":\n";

    size_t n = M.rows();
    size_t m = M.cols_count();

    std::cout << std::fixed << std::setprecision(6);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {

            std::cout << std::setw(12) << M[j][i];
        }
        std::cout << "\n";
    }
    std::cout << std::defaultfloat;
}

// Print vector with formatting
inline void print_vector(const pp::vector& v, const std::string& name = "") {

    if (!name.empty())
        std::cout << name << ":\n";

    std::cout << std::fixed << std::setprecision(6);

    for (size_t i = 0; i < v.size(); i++) {
        std::cout << std::setw(12) << v[i] << "\n";
    }
    std::cout << std::defaultfloat;
}

}