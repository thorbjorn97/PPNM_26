// "harm.h" header file: Declares the names of the functions implemented in "harm.cc"

#pragma once  // prevent multiple inclusion of this header file

// ============================================================================
// Task 1: Prepare the subroutine to be run in a thread
// ============================================================================

// Data structure storing the index range [a,b) assigned to a thread
// together with the corresponding partial harmonic sum:
struct data {
    int a, b;    // index range [a,b) of terms in the harmonic series
    double sum;  // resulting partial harmonic sum over the index range [a, b)
};

// Worker function that computes the partial harmonic sum over the index
// range [a,b) and stores the result in arg.sum:
void harm(data& arg);

// ============================================================================

// end