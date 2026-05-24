// utils.h

#pragma once

#include <iomanip>
#include <iostream>

namespace pp {

// === Print functions ===

// Print section header
inline void section(const std::string& title) {
    std::cout << "\n\n"
              << "================================================================================\n"
              << title << "\n"
              << "================================================================================\n";
}

// Print substep header
inline void substep(const std::string& title) {
    std::cout << "\n"
              << title << "\n"
              << "----------------------------------------\n";
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

}
