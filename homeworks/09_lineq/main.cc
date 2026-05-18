#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "linalg_utils.h"
#include "matrix.h"
#include "qr.h"
#include "vector.h"

int main(int argc, char* argv[]) {
    using namespace pp;

    // Benchmark mode
    if (argc == 3 && std::string(argv[1]) == "-size") {
        size_t N = std::stoul(argv[2]);

        matrix A = random_matrix(N, N);

        qr decomposition(A);

        return 0;
    }

    // Normal test mode
    using namespace pp;

    std::cout << "\nInitiating tests...\n";

    // ============================================================================================
    // Testing QR decomposition (Task A.2)
    // ============================================================================================
    section("TEST 1: QR DECOMPOSITION [Task A.2]");

    substep("Input");

    size_t n = 6;
    size_t m = 4;
    matrix A = random_matrix(n, m);
    print_matrix(A, "Matrix A");

    substep("QR Decomposition");

    qr decomposition_A(A);
    print_matrix(decomposition_A.getQ(), "Matrix Q");
    print_matrix(decomposition_A.getR(), "Matrix R");

    substep("Validation");

    std::cout << "R is upper triangular: "
              << (is_upper_triangular(decomposition_A.getR()) ?
                 "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m")
              << "\n";

    std::cout << "Q is orthonormal (Q^{T}Q = I): "
              << (check_orthonormal(decomposition_A.getQ()) ?
                 "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m")
              << "\n";

    std::cout << "QR reconstructs A (A = QR): "
              << (check_reconstruction(decomposition_A.getQ(), decomposition_A.getR(), A) ?
                 "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m")
              << "\n";

    // ============================================================================================
    // Testing linear solver (Task A.3)
    // ============================================================================================

    section("TEST 2: LINEAR SOLVER [Task A.3]");

    substep("Input");

    size_t dim = 4;
    matrix B = random_matrix(dim, dim);
    vector b = random_vector(dim);
    print_matrix(B, "Matrix B");
    print_vector(b, "Vector b");

    substep("Solving QRx = b");

    qr decomposition_B(B);  // create QR decomposition of B

    vector x = decomposition_B.solve(b);  // solve QRx = b

    print_vector(x, "Solution x");

    substep("Computing Bx and comparing to b");

    vector Bx = matvec(B, x);

    print_vector(Bx, "Vector Bx");
    print_vector(b, "Vector b");

    substep("Validation");

    bool solve_ok = true;

    for (size_t i = 0; i < dim; i++) {
        if (std::abs(Bx[i] - b[i]) > 1e-8) {
            solve_ok = false;
            break;
        }
    }

    std::cout << "Solve works as expected (Bx = b): "
              << (solve_ok ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m")
              << "\n";

    // ============================================================================================
    // Testing determinant (Task A.4)
    // ============================================================================================

    section("TEST 3: DETERMINANT [Task A.4]");

    substep("Input");
    print_matrix(B, "Matrix B");

    substep("QR Decomposition");

    double det_qr = decomposition_B.det();

    matrix Q = decomposition_B.getQ();
    matrix R = decomposition_B.getR();

    print_matrix(Q, "Matrix Q");
    print_matrix(R, "Matrix R");

    substep("Computing det(QR) and prod(diag(R))");

    double det_R = 1.0;
    for (size_t i = 0; i < R.cols_count(); i++) {
        det_R *= R[i][i];
    }

    std::cout << "det(QR) = " << det_qr << "\n";
    std::cout << "prod(diag(R)) = " << det_R << "\n";

    substep("Validation (consistency check)");

    bool det_ok = std::abs(std::abs(det_qr) - std::abs(det_R)) < 1e-8;

    std::cout << "det(QR) = prod(diag(R)): "
              << (det_ok ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m")
              << "\n";

    // ============================================================================================
    // Testing matrix inverse (Task B)
    // ============================================================================================

    section("TEST 4: MATRIX INVERSE [Task B]");

    substep("Input");

    matrix C = random_matrix(dim, dim);
    print_matrix(C, "Matrix C");

    substep("Computing matrix inverse");

    qr inv_qr(C);
    matrix Cinv = inv_qr.inverse();
    print_matrix(Cinv, "Matrix C^{-1}");

    substep("Validation");

    matrix I = matmul(C, Cinv);

    std::cout << "C^{-1} is identity: "
              << (check_identity(I) ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m")
              << "\n";

    // ============================================================================================
    // Plotting (Task C) [see plot.gpi]
    // ============================================================================================

    section("TEST 5: PLOTTING AND FITTING [Task C]");

    return 0;
}