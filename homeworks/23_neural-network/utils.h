// utils.h

#pragma once

#include <iomanip>
#include <iostream>

#include "vector.h"
#include "matrix.h"

namespace pp {

// Print section header
inline void section(const std::string& title) {
    std::cout << "\n\n"
              << std::string(80, '=') << "\n"
              << title << "\n"
              << std::string(80, '=') << "\n";
}

// Print substep header
inline void substep(const std::string& title) {
    std::cout << "\n"
              << title << "\n"
              << std::string(40, '-') << "\n";
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
inline void print_vector(const pp::vector& v, const std::string& name = "", const int prec = 6) {

    if (!name.empty())
        std::cout << name << ":\n";

    auto old_flags = std::cout.flags();
    auto old_prec  = std::cout.precision();

    std::cout << std::fixed << std::setprecision(prec);

    for (size_t i = 0; i < v.size(); i++) {
        std::cout << std::setw(12) << v[i] << "\n";
    }

    std::cout.flags(old_flags);
    std::cout.precision(old_prec);
}

// Simple boolean PASS/FAIL printer with colors
inline void print_pass_fail(bool pass) {
    if (pass)
        std::cout << "\033[1;32mPASS\033[0m\n";
    else
        std::cout << "\033[1;31mFAIL\033[0m\n";
}

}
