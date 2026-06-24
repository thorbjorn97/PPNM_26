// qr.h [From homework 09_lineq]

#pragma once

#include <stdexcept>
#include <cstddef>
#include <cmath>

#include "matrix.h"
#include "vector.h"

namespace pp {

class qr {
private:
    matrix Q;
    matrix R;

public:
    qr(const matrix& A) {

        size_t n = A.rows();
        size_t m = A.cols_count();

        if (n < m) {
            throw std::runtime_error("QR requires n >= m");
        }

        Q = matrix(n, m);
        R = matrix(m, m);

        for (size_t j=0;j<m;j++) {

            Q[j]=A[j];

            for (size_t i=0;i<j;i++) {

                R[i][j]=Q[i].dot(Q[j]);

                Q[j]=Q[j]-Q[i]*R[i][j];
            }

            R[j][j]=Q[j].norm();

            if(R[j][j]<1e-14)
                throw std::runtime_error("Linear dependence");

            Q[j]=Q[j]*(1.0/R[j][j]);
        }
    }

    const matrix& getQ() const { return Q; }
    const matrix& getR() const { return R; }

    vector solve(const vector& b) const {

        size_t n = Q.rows();
        size_t m = Q.cols_count();

        if (b.size() != n) {
            throw std::runtime_error("Dimension mismatch in solve()");
        }

        vector y(m);

        for (size_t i = 0; i < m; i++) {
            y[i] = Q[i].dot(b);
        }

        vector x(m);

        for(size_t i = m; i-- > 0;) {

            double sum = 0.0;

            for (size_t j = i + 1; j < m; j++) {
                sum += R[i][j] * x[j];
            }

            double diag = R[i][i];

            if (std::abs(diag) < 1e-14) {
                throw std::runtime_error("Singular matrix in solve()");
            }

            x[i] = (y[i] - sum) / diag;
        }

        return x;
    }

    double det() const {
    
        if (Q.rows() != Q.cols_count()) {
            throw std::runtime_error(
                "Determinant only defined for square matrices"
            );
        }

        /* A = QR
        
        Since R is upper triangular:
        det(R) = product of diagonal elements.
        
        In this QR construction the diagonal entries
        R[j][j] are positive, so det(Q)=1 and
        det(A)=det(R). */

        size_t m = R.cols_count();

        double det = 1.0;

        for (size_t i = m; i-- > 0;) {
            det *= R[i][i];
        }

        return det;
    }

    matrix inverse() const {

        size_t n = Q.rows();
        size_t m = Q.cols_count();

        if (n != m) {
            throw std::runtime_error(
                "Inverse only defined for square matrices"
            );
        }

        matrix B(n, n);

        for (size_t j = 0; j < n; j++) {

            vector e(n); // create basis vector e_j

            e[j] = 1.0;

            vector x = solve(e); // solve A x = e_j

            B[j] = x; // store solution as column j
        }

        return B;
    }

};

}