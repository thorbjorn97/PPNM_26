// main.cc

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "evd.h"
#include "matrix.h"
#include "vector.h"
#include "linalg_utils.h"

using namespace pp;

int main(int argc,char** argv)
{
    // ============================================================================================
    // Task A (6 points): Jacobi diagonalization with cyclic sweeps
    // ============================================================================================
    section("Task A (6 points): Jacobi diagonalization with cyclic sweeps");

    substep("Input:");

    int n = 5;

    matrix A = random_symmetric(n);
    matrix A0 = A; // store original

    print_matrix(A, "Matrix A");

    substep("Jacobi diagonalization:");

    auto [w, V] = jacobi(A); // use Jacobi diagonalization to compute eigenvalues and eigenvectors 

    // Build diagonal matrix D
    matrix D(n,n);
    for(int i = 0; i < n; i++)
        D(i,i) = w[i];

    print_matrix(V, "Matrix V");
    print_matrix(D, "Matrix D");
    print_vector(w, "Vector w");

    substep("Validation:");

    // Prepare checks
    matrix VT(n,n);
    for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
        VT(i,j) = V(j,i);

    // Check 1: V^T A V == D
    matrix A_diag = VT * A0 * V;

    // Check 2: V D V^T == A
    matrix A_rec = V * D * VT;

    // Check 3: V^T V == I
    matrix I1 = VT * V;

    // Check 4: V V^T == I
    matrix I2 = V * VT;

    // Print results and auto-grade
    std::cout << "\n||V^T A V - D|| = " << frob_diff(A_diag, D) << "\n";
    std::cout << "||V D V^T - A|| = " << frob_diff(A_rec, A0) << "\n";
    std::cout << "||V^T V - I|| = " << frob_diff(matrix::identity(n), I1) << "\n";
    std::cout << "||V V^T - I|| = " << frob_diff(matrix::identity(n), I2) << "\n";
    
    bool pass = (frob_diff(I1, matrix::identity(n)) < 1e-10) &&
                (frob_diff(I2, matrix::identity(n)) < 1e-10) &&
                (frob_diff(A0, A_rec) < 1e-10) &&
                (frob_diff(A_diag, D) < 1e-10);
    
    std::cout << "\nAUTO-GRADE A: " << (pass ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m") << "\n";

    // ============================================================================================
    // Task B (3 points): Hydrogen atom, s-wave radial Schrödinger equation on a grid
    // ============================================================================================
    section("Task B (3 points): Hydrogen atom, s-wave radial Schrödinger equation on a grid");
    // ====================================================
    // PARAMETERS FOR BASE PROBLEM AND WAVEFUNCTION PLOTS
    // ====================================================
    substep("Parameters (for base problem and wavefunction plots)");

    // Parameters (can be overridden by command line arguments)
    double rmax = 40.0;
    double dr = 0.20;

    // Parse command line arguments
    for(int i=1;i<argc;i++)
    {
        std::string arg = argv[i];
        if(arg=="-rmax") rmax = std::stod(argv[++i]);
        else if(arg=="-dr") dr = std::stod(argv[++i]);
    }

    double exact_energy = -0.5;
    double tol = 1e-2;

    std::cout << "\nrmax = " << rmax <<"\n";
    std::cout << "dr = " << dr << "\n\n";
    std::cout << "exact energy (Hartree) = " << exact_energy << "\n";
    std::cout << "tolerance = " << tol << "\n";

    // ====================================================
    // PARAMETERS FOR CONVERGENCE PLOTS
    // ====================================================
    substep("Parameters (for convergence plots)");
    
    double rmax_fixed = 10.0;
    double dr_fixed=0.05;

    // Create .gpvars file for fixed rmax and dr
    std::ofstream gpvars("plot_vars.gp");
    gpvars << "\nrmax_fixed = " << rmax_fixed << "\n";
    gpvars << "dr_fixed = " << dr_fixed << "\n";
    gpvars.close();

    std::cout << "\nrmax_fixed = " << rmax_fixed <<"\n";
    std::cout << "dr_fixed = " << dr_fixed << "\n";

    // ====================================================
    // SOLVE BASE PROBLEM
    // ====================================================
    substep("Solve eigenvalue problem");

    // Build grid
    int npoints = (int)(rmax/dr) - 1;
    vector r(npoints);
    for(int i=0;i<npoints;i++) r[i] = dr*(i+1);

    // Build and diagonalize Hamiltonian and sort eigenvalues
    matrix H = hydrogen_H(rmax, dr);
    auto [e, V_H] = jacobi(H);
    sort_eigen(e, V_H);

    // Fix sign ambiguity
    for(int k = 0; k < npoints; k++)
    {
        // find first non-zero component
        for(int i = 0; i < npoints; i++)
        {
            if(std::abs(V_H(i,k)) > 1e-12)
            {
                if(V_H(i,k) < 0)
                {
                    for(int j = 0; j < npoints; j++)
                        V_H(j,k) *= -1.0;
                }
                break;
            }
        }
    }

    // Lowest eigenvalues
    std::cout << "\nLowest eigenvalues:\n";
    for(int i = 0; i < 5; i++)
        std::cout << "E[" << i << "] = " << e[i] << "\n";

    double E0 = e[0];
    std::cout << "\nLowest eigenvalue: E0 = " << E0 << "\n";

    // ====================================================
    // DR CONVERGENCE
    // ====================================================
    substep("Convergence in dr");

    std::ofstream out_dr("dr_convergence.dat");

    std::vector<double> e_vs_dr;
    std::vector<double> dr_grid;

    for(double dr_scan = 1.0; dr_scan >= 0.02; dr_scan -= 0.02)
    {
        double e0 = ground_state(rmax_fixed, dr_scan);

        e_vs_dr.push_back(e0);
        dr_grid.push_back(dr_scan);

        out_dr << dr_scan << " " << e0 << "\n";
    }

    // Summary:
    std::cout << "\n[dr convergence for rmax=" << rmax_fixed << "]\n";
    std::cout << "start: dr = " << std::setw(5) << dr_grid.front()
            << "   E0 = " << std::setw(10) << e_vs_dr.front() << "\n";
    std::cout << "end:   dr = " << std::setw(5) << dr_grid.back()
            << "   E0 = " << std::setw(10) << e_vs_dr.back() << "\n";

    bool dr_ok = std::abs(e_vs_dr.back() - exact_energy) < tol;
    std::cout << "dr convergence: " << (dr_ok ? "PASS" : "FAIL") << "\n";

    // ====================================================
    // RMAX CONVERGENCE
    // ====================================================
    substep("Convergence in rmax");

    std::ofstream out_rmax("rmax_convergence.dat");

    std::vector<double> e_vs_rmax;
    std::vector<double> rmax_grid;

    for(double rmax_scan=3; rmax_scan<=10; rmax_scan+=0.2)
    {
        double e0 = ground_state(rmax_scan, dr_fixed);

        e_vs_rmax.push_back(e0);
        rmax_grid.push_back(rmax_scan);

        out_rmax << rmax_scan << " " << e0 << "\n";
    }

    // Summary:
    std::cout << "\n[rmax convergence for dr=" << dr_fixed << "]\n";
    std::cout << "start: rmax = " << std::setw(3) << rmax_grid.front()
            << "   E0 = " << std::setw(10) << e_vs_rmax.front() << "\n";
    std::cout << "end:   rmax = " << std::setw(3) << rmax_grid.back()
            << "   E0 = " << std::setw(10) << e_vs_rmax.back() << "\n";
    
    bool rmax_ok = std::abs(e_vs_rmax.back() - exact_energy) < tol;
    std::cout << "rmax convergence: " << (rmax_ok ? "PASS" : "FAIL") << "\n";

    // ====================================================
    // WAVEFUNCTIONS
    // ====================================================
    std::ofstream wave("wavefunctions.dat");

    for(int i=0;i<npoints;i++)
    {
        // numerical wavefunctions (reduced radial, normalized)
        double f0_num = V_H(i,0) / std::sqrt(dr);
        double f1_num = V_H(i,1) / std::sqrt(dr);
        double f2_num = V_H(i,2) / std::sqrt(dr);
        
        // exact hydrogen s-wave reduced radial functions
        double f0_exact =
            2*r[i]*std::exp(-r[i]);

        double f1_exact =
            (1.0 / (2.0 * std::sqrt(2.0))) *
            r[i] * (2.0 - r[i]) *
            std::exp(-r[i] / 2.0);

        double f2_exact =
            (2.0 / (81.0 * std::sqrt(3.0))) *
            r[i] * (27.0 - 18.0 * r[i] + 2.0 * r[i] * r[i]) *
            std::exp(-r[i] / 3.0);

        wave
        << r[i] << " "
        << f0_num << " " << f0_exact << " "
        << f1_num << " " << f1_exact << " "
        << f2_num << " " << f2_exact
        << "\n";
    }

    // ====================================================
    // AUTO-GRADING
    // ====================================================
    substep("Grading");

    bool passB =
        std::abs(E0 - exact_energy) < tol &&
        E0 < 0 &&
        e[1] > e[0] &&
        dr_ok &&
        rmax_ok;

    std::cout << "\nAUTO-GRADE B: "
            << (passB ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m")
            << "\n";

    // ============================================================================================
    // Task C (1 point): Scaling and optimization
    // ============================================================================================
    section("Task C (1 point): Scaling and optimization");

    std::cout << "\npending...\n\n";

    return 0;
}