<!-- 
// README.md
-->

# The exam question \#29 (verbatim) 

**Quasi-Newton mininization with Broyden's update of the Hessian matrix**

<u>Task</u>

Implement a function with the signature

    vector broyden(Func<vector,double> f, vector x, double acc)

that takes the function `f` and runs the quasi-Newton algorithm from the starting point `x`, using the Broyden's update of the Hessian matrix until the accuracty goal acc is reached (see the book for the details).

Test your implementation on some functions with known minima.

Apply your implementation to a more complicated problem

Implement also the symmetrized Broyden's update and check whether it is any better.

# Project overview

My answer is structured according to the outline below. I chose to define both variants, `broyden()` and `broyden_symmetrized()`, of the Quasi-Newton solver right from the start (**Phase 1**). I did so in order to more conveniently enable the testing on "some functions with known minima" (**Phase 2**) and application "to a more complicated problem" (**Phase 3**) to be done in parallel, allowing these tests to be included as part of the comparison to investigate whether the symmetrized Broyden's update version "is any better" (**Phase 2-4**).

                                        OUTLINE
    ================================================================================
     PHASE 1: IMPLEMENTATION OF SOLVERS
    ================================================================================
    TASK 1 - Quasi-Newton Solver with Standard Broyden Update
    TASK 2 - Quasi-Newton Solver with Symmetrized Broyden Update

    ================================================================================
     PHASE 2: TEST ON FUNCTIONS WITH KNOWN MINIMA
    ================================================================================
    TASK 3 - Tests on Rosenbrock Valley Function in 2D
      4.1 - BASIC TEST ON ROSENBROCK
      3.2 - PERFORMANCE FROM SELECTED INITIAL POINTS
      3.3 - ROSENBROCK CONVERGENCE EFFICIENCY GRID SWEEP
    TASK 4 - Tests on Hummelblau Function
      4.1 - BASIC TEST ON HIMMELBLAU
      4.2 - PERFORMANCE FROM SELECTED INITIAL POINTS
      4.3 - HIMMELBLAU BASINS OF ATTRACTION GRID SWEEP

    ================================================================================
     PHASE 3: APPLICATION TO A MORE COMPLICATED PROBLEM
    ================================================================================
    TASK 5 - Higgs parameter optimization
      5.1 - PREPARATION OF HIGGS DATA FROM FILE
      5.2 - FIT OF BREIT-WIGNER TO HIGGS DATA
      5.3 - PLOT OF FIT CURVES
      5.4 - PLOT OF CONVERGENCE HISTORY

    ================================================================================
     PHASE 4: DIMENSIONAL BENCHMARKING
    ================================================================================
    TASK 6 - Dimensional Benchmarking on the Generalized Rosenbrock Function
      6.1 - DIMENSIONAL SCALING PERFORMANCE
      6.2 - EXTERNAL EXECUTION TIME BENCHMARKING (VIA GNU TIME)

# System prerequisites

To compile and run this program on a system, it must have:
* **Compiler:** A C++ compiler supporting the `C++23` standard.
* **Visualization:** `Gnuplot` installed and available to generate plots.

# Build & Execution Instructions

TODO

# Directory Structure Map

TODO

# Summary of Key Findings

TODO