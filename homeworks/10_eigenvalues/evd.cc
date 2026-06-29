// evd.cc

#include <cmath>
#include <iostream>

#include "evd.h"

namespace pp{

// =========================================================
// Task 1: A ← A * J
// =========================================================
void timesJ(matrix& A, int p, int q, double theta){
    double c = std::cos(theta);
    double s = std::sin(theta);

    for(size_t i=0;i<A.rows();i++){
        double aip = A(i, p);
        double aiq = A(i, q);

        A(i, p) = c*aip - s*aiq;
        A(i, q) = s*aip + c*aiq;
    }
} // timesJ

// =========================================================
// Task 2: A ← J^T * A
// =========================================================
void Jtimes(matrix& A, int p, int q, double theta)
{
    double c = std::cos(theta);
    double s = std::sin(theta);

    for(size_t j=0; j<A.cols_count(); j++)
    {
        double apj = A(p, j);
        double aqj = A(q, j);

        A(p, j)= c*apj + s*aqj;
        A(q, j)=-s*apj + c*aqj;
    }
} // Jtimes

// =========================================================
// Task 3: Jacobi diagonalization
// =========================================================
std::tuple<vector,matrix> jacobi(matrix A){// "A" is a copy
	int n = static_cast<int>(A.rows());
    matrix V = matrix::identity(n);
    vector w(n);

    bool changed;
    int sweep = 0;
    do{
        sweep++;
        if(sweep > 1000){
            std::cerr << "Jacobi did not converge after 1000 sweeps\n";
            break;
        }

	    changed=false;
	    for(int p=0;p<n-1;p++) 
	    for(int q=p+1;q<n;q++){
		    double apq=A(p, q), app=A(p, p), aqq=A(q, q);
		    double theta = 0.5*std::atan2(2*apq,aqq-app);
		    
            if(std::abs(apq) > 1e-12) // do rotation
                {
                    changed = true;
                    timesJ(A,p,q, theta); // A ← A * J
                    Jtimes(A,p,q,-theta); // A ← J^T * A  (do rotation on A)
                    timesJ(V,p,q, theta); // V ← V * J    (update eigenvectors V)
                }
	    }
    
    }while(changed);

    // extract and store eigenvalues and eigenvectors in w and V
    for(int i = 0; i < n; i++) { w[i] = A(i, i); } // (copy diagonal elements into w)
    return std::make_tuple(w, V);
} // jacobi

} // namespace pp
