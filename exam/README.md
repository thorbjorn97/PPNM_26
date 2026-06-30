<!-- 
// README.md
-->

# The exam question \#29 (verbatim) 

**Quasi-Newton mininization with Broyden's update of the Hessian matrix**

<ins>Task</ins>

Implement a function with the signature

```cpp
vector broyden(Func<vector,double> f, vector x, double acc)
```

that takes the function `f` and runs the quasi-Newton algorithm from the starting point `x`, using the Broyden's update of the Hessian matrix until the accuracty goal acc is reached (see the book for the details).

Test your implementation on some functions with known minima.

Apply your implementation to a more complicated problem

Implement also the symmetrized Broyden's update and check whether it is any better.

# Outline of my answer

My answer is structured according to the outline below. I chose to define both variants, `broyden()` and `broyden_symmetrized()`, of the Quasi-Newton solver right from the start (**Phase 1**). I did so in order to more conveniently enable the testing on "some functions with known minima" (**Phase 2**) and application "to a more complicated problem" (**Phase 3**) to be done in parallel, allowing these tests to be included as part of the comparison to investigate whether the symmetrized Broyden's update version "is any better" (**Phase 2-4**).

                                        OUTLINE
    ================================================================================
     PHASE 1: IMPLEMENTATION OF MINIMIZERS
    ================================================================================
    TASK 1 - Quasi-Newton Minimizer with Standard Broyden Update
    TASK 2 - Quasi-Newton Minimizer with Symmetrized Broyden Update

    ================================================================================
     PHASE 2: TEST ON FUNCTIONS WITH KNOWN MINIMA
    ================================================================================
    TASK 3 - Tests on Rosenbrock Valley Function in 2D
      3.1 - BASIC TEST ON ROSENBROCK
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

# Short description of what I did

### PHASE 1
I implemented both of the Quasi-Newton minimizer variants, as `broyden()` (Task 1) and `broyden_symmetrized()` (Task 2), respectively, in `broyden.cc`. I followed the guidance provided in *The Book* (Chapters 9.3 and 10.2.3). Both minimizers have a minimum step size of `lambda_min = 1.0/1024`, an upper step count of `max_steps = 10000`, and use, for efficiency, the *inverse* Hessian matrix **B**, initialized to the identity matrix **I**. To facilitate downstream analysis, I integrated a logging helper into each minimizer to track a set of key properties (step count, final function value fx, final gradient norm ‖∇fx‖, and exit reason).

In the remaining phases (2-4), I then performed a series of tests on the two implemented minimizers side-by-side to investigate whether the symmetrized version is any better (i.e. if preserving Hessian symmetry offers measurable advantages). In all tests, I set the tolerance for the gradient norm ‖∇fx‖ (i.e. accuracty goal acc) to be `acc_tol = 1e-5` and, when relevant, the coordinate distance tolerance to the expected minimum to be `coord_tol = 1e-3`.

### PHASE 2

First, I tested the minimizers on some functions with known minima, specifically *The 2D Rosenbrock Valley Function* (Task 3) and *The Himmelblau Function* (Task 4). Both are 2D problems. The test on each of these functions was divided into three parts:

### (1) Basic single initial point verification

A basic test on a single (relatively easy) initial point to verify basic functionality of the minimizers. The resulting convergence paths through (x,y) space were saved to datafiles and plotted on a contour map of the functions (see `rosenbrock_convergence_base.svg` and `himmelblau_convergence.svg`).

* **Conclusion**: Both minimizers work as they should, and have, for each test functions, seemingly comparable performance for the selected initial point. However, their convergence paths differ notably; `broyden()` exhibits erratic behavior, taking a sharp, sudden jump across the steep walls of the Rosenbrock Valley, whereas `broyden_symmetrized()` maps a significantly smoother and more stable path to the minimum.

### (2) Diverse set of initial points

A test on a set of diverse initial points with different properties to extend the comparison. Here I also measured the execution time the quick-and-dirty way (internally) using `std::chrono`. A table of the test results was generated, as well as datafiles of the convergence paths through (x,y) space.

* **Conclusion**: For the selected sample of points on Rosenbrock, neither minimizer emerged as a definitive winner for local points, yielding nearly identical step counts. For the selected sample of points on Himmelblau, however, `broyden_symmetrized()` was consistently converging faster and with fewer steps. Both minimizers failed explicitly at the extreme initialization point (99,99), triggering immediate divergence towards infinity. This reflects the limitation of the initial identity Hessian approximation **B=I** without further upper constraints on the step sizes; the minimizers might take unconstrained, massive overshoots into numerical overflow.

### (3) 10,000-point uniform grid sweeps

To eliminate initialization bias, I executed a 100x100 grid sweep around the analytical minima, visualizing the performance landscapes via pixelated spatial maps, with each pixel representing the location of an initial point (see `rosenbrock_basin.svg` and `himmelblau_basins.svg`).

- In `rosenbrock_basin.svg`, each initial point pixel is colorcoded by the number of steps taken to converge to the known minimum (1,1). This is done to compare *step efficiency*.
    
  * **Conclusion**: On average, `broyden_symmetrized()` clearly converges with fewer steps than `broyden()` on the chosen grid sweep. The majority of its successful runs fall into the highest efficiency tiers (≤ 30 steps), with very few runs above 50 steps. The number of failed points is also significantly lower for `broyden_symmetrized()` (13 versus 162).
  
- In `himmelblau_basins.svg`, each initial point pixel is colorcoded by which of the four analytical minima it converges to. This is done to compare *basin fidelity*, i.e. the ability to converge to the mathematically expected (deepest descend) natural geometric basin, which is also plotted for comparison (leftmost).
    
  * **Conclusion**: On average, `broyden_symmetrized()` is only slightly better at convergning to the expected natural geometric basin, with a succes rate of 71.47% (versus 68.66%) for the chosen grid sweep. However, the number of failed points is, interestingly, a bit higher for `broyden_symmetrized()` (18 versus 4). The fidelity scores remain low for both minimizers as expected; since Quasi-Newton methods build historical, path-dependent matrix updates rather than executing pure infinitesimal steepest descent, their path memory naturally causes them to sometimes drift across the narrow boundaries separating competing basins of attraction.

### PHASE 3
Next (Task 5), I tested the minimizers on a more complicated problem, namely the Higgs parameter optimization problem corresponding to Task B in homework 21_minimum. This is a noisy, physical optimization problem in 3D. By minimizing the Deviation function D(m, Γ, A) on the 30 points from the Higgs dataset, taking into account uncertainties, the three parameters (m, Γ, A) were estimated. Resulting fits were plotted (see `higgs_fit`). I also plotted the convergence history of the Deviation function D(m, Γ, A) for both minimizers (see `higgs_convergence.svg`).

**Conclusion**: Moving from ideal functions to noisy empirical data in 3D exposes a massive performance gap. While both minimizers successfully arrive at nearly identical optimal parameters, `broyden()` struggles in the noisy parameter space, requiring 734 steps to converge, whereas `broyden_symmetrized()` converged in just 20 steps (a ~36x speedup).

### PHASE 4

Finally (Task 6), I evaluated the structural scalability of both algorithms on the Generalized Rosenbrock function across expanding dimensions (N = 2, 4, 8, 16, 32). The execution profiles and function evaluation overheads were captured externally via GNU `time` and plotted in `plot_timing.svg`.

**Conclusion**: The dimensional benchmark reveals an immediate architectural breakdown in the standard variant. While both algorithms scale identically at N=2, Standard Broyden collapses at 4≤N, with its workload exploding from 35 steps to a massive plateau of over 5,800 to 7,300 steps, forcing more than 300,000 expensive objective function evaluations. In stark contrast, `broyden_symmetrized()` exhibits exceptional scaling stability, successfully taming the high-dimensional spaces to resolve the N=32 problem in a highly optimized 199 steps (requiring fewer than 7,000 function calls).

### Concluding Remark

The empirical evidence is definitive: **`broyden_symmetrized()` is fundamentally superior.** While the standard update is highly brittle (oscillating heavily when navigating topological irregularities, locking up in dimensions 4≤N, and struggling on noisy data), the symmetric update exhibit better stability and noise resilience, faster execution speeds, and exceptional dimensional scaling.

### Self-evaluation of the project on the scale [0,10]

10


# System prerequisites

To compile and run this program on a system, it must have:
* **Compiler:** A `g++` with support for the `C++23` standard.
* **Plotting Utility:** `gnuplot` installed and available to generate plots.
* **Profiling Utility:** GNU `time` located at `/usr/bin/time`.
* **Shell Utilities:** Standard Unix utilities (`awk`, `seq`, `tee`)

# Build and execution instructions

## Quick start
For a guaranteed clean, end-to-end test, execute

```bash
make clean; make
```
(or alternatively: `make dev`)

This removes all existing output files, recompiles the source code, runs a fresh default execution, and generates all benchmark metrics and plots from scratch.

## Basic commands

| Command | Description |
| :--- | :--- |
| **`make all`** / **`make`** | **Incremental run:** Builds missing components, benchmarks, and plots. |
| **`make main`** | Compiles source files into the `main` binary executable. |
| **`make run`** | Executes the main program `./main` and mirrors the terminal output to `out.txt`. |
| **`make plot`** | Runs the script `plot.gpi` which generates all plots exept `plot_timing.svg`. |
| **`make timing_results.dat`** | Runs profiling loop (Dim 2 to `MAX_DIM`) with a live progress bar. |
| **`make plot_timing.svg`** | Generates the execution time profiling plot `plot_timing.svg`. |
| **`make clean`** | Removes all compiled binaries, intermediate object files, data logs, and plots. |
| **`make dev`** / **`make clean; make`** | **Full reset + full run:** Cleans workspace, recompiles, benchmarks, and plots. |

*Note: The benchmark upper dimension limit N can be specified in any command by appending `MAX_DIM=N` (e.g., `make dev MAX_DIM=80`).*

# Directory structure map

      [Source Files - x12]
    |-- broyden.cc            # Standard and Symmetrized Broyden solver implementation
    |-- broyden.h             # Standard and Symmetrized Broyden solver definition
    |-- higgs.data.txt        # Data from Higgs experiment (from homework 21_minimum)
    |-- linalg_utils.h        # Custom linear algebra utilities
    |-- main.cc               # Application entry point and CLI parser
    |-- Makefile              # Build system configuration
    |-- matrix.h              # Custom matrix math utilities
    |-- plot.gpi              # Gnuplot script for all plots except 'plot_timing.svg'
    |-- README.md             # Current file
    |-- rose_benchmark.h      # Generalized Rosenbrock function definition
    |-- utils.h               # Custom misc. utilities (mostly printing)
    |-- vector.h              # Custom vector math utilities
      [Generated Files - x45]
    |-- out.txt               # Mirror of the terminal output
    |-- main                  # Compiled C++ binary executable
    |-- *.o                   # Intermediate C++ object compilation files (x2)
    |-- *.dat                 # Raw data logs (x34)
    |-- *.svg                 # Plot files (x7)
