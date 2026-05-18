// evd.h

#pragma once

#include <tuple>

#include "matrix.h"
#include "vector.h"

namespace pp{

std::tuple<vector, matrix> jacobi(matrix A);

void timesJ(matrix& A, int p, int q, double theta);
void Jtimes(matrix& A, int p, int q, double theta);

}