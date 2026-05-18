// matrix.h

#pragma once

#include <cstddef>
#include <vector>

#include "vector.h"

namespace pp {

class matrix {
private:
    std::vector<vector> cols;  // column-major

public:
    matrix() = default;

    matrix(size_t rows, size_t cols_) { cols.resize(cols_, vector(rows)); }

    size_t rows() const { return cols.empty() ? 0 : cols[0].size(); }

    size_t cols_count() const { return cols.size(); }

    vector& operator[](size_t j) { return cols.at(j); }

    const vector& operator[](size_t j) const { return cols.at(j); }

    const std::vector<vector>& data() const { return cols; }

    static matrix identity(size_t n) {
        matrix I(n, n);
        for (size_t i = 0; i < n; i++)
            I(i, i) = 1.0;
        return I;
    }

    double& operator()(size_t i, size_t j) { return cols[j][i]; }

    const double& operator()(size_t i, size_t j) const { return cols[j][i]; }
    
};

}  // namespace pp