// "harm.cc" source file: Implements the function declared in "harm.h"

#include "harm.h"  // Includes the content of the header file "harm.h"

// ============================================================================
// Task 1: Prepare the subroutine to be run in a thread
// ============================================================================

// Compute the partial harmonic sum over the index range [a,b) assigned to
// this thread in main.cc and store the result in arg.sum:
void harm(data& arg) {
    // "data& arg" means that arg is a reference to a "struct data" object with
    // fields a, b, sum (cf. harm.h) created in main.cc (arg ≡ params[i])
    int a = arg.a;   // set a as a copy of params[i].a (cf. main.cc)
    int b = arg.b;   // set b as a copy of params[i].b (cf. main.cc)
    double sum = 0;  // initialize the sum to zero.
    for (int i = a; i < b; i++) {
        sum += 1.0 / i;
    }  // compute the sum Σ_{i=a}^{b-1} 1/i and store it in sum
    arg.sum = sum;  // store sum in arg.sum (write it back to params[i].sum)
}

// ============================================================================

// end