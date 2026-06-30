// main.cc

#include <iostream>
#include <cmath>
#include <iomanip>
#include <functional>
#include <string>
#include <chrono>
#include <fstream>

#include "utils.h"
#include "vector.h"
#include "matrix.h"
#include "broyden.h"
#include "rose_benchmark.h"

using namespace pp;

int main(int argc, char* argv[]) {

    // Accuracy tolerances applied in all tests
    double acc_tol = 1e-5;     // tolerance for the gradient norm ‖∇fx‖
    double coord_tol = 1e-3;   // tolerance for coordinate distance to true/expected minimum

    // [For Task 6] EXTERNAL BENCHMARKING / TIMING EXECUTION MODE
    // Used by the Makefile loop to capture raw execution times via automated scripts.
    // Expects: ./main <dimension> <method_flag>
    if (argc > 2) {
        try {
            const size_t dim = std::stoul(argv[1]);
            const std::string method = argv[2];

            // Initialize the standard non-symmetric Rosenbrock start vector
            // x_start = [-1.2, 1.0, -1.2, 1.0, ...]
            vector n_start(dim);
            for (size_t i = 0; i < dim; ++i) n_start[i] = (i % 2 == 0) ? -1.2 : 1.0;

            int counter = 0; // counts the number of function calls
            auto f = [&counter](const vector& v) { return RosenbrockBenchmarker::rosenbrockNd(v, counter); };

            if (method == "std")      broyden(f, n_start, acc_tol);
            else if (method == "sym") broyden_symmetrized(f, n_start, acc_tol);
            return 0;
        }
        catch (const std::exception& e) {
            std::cerr << "Benchmark error: " << e.what() << "\n";
            return 1;
        }
    }

    // ================================================================================
    //                      PHASE 1: IMPLEMENTATION OF SOLVERS
    // ================================================================================

    // ================================================================================
    // TASK 1 - Implementation of Quasi-Newton mininization with Broyden's update (Standard Broyden)
    // ================================================================================
    section("TASK 1 - Implementation of Quasi-Newton mininization with Broyden's update (Standard Broyden)");

    std::cout << "[INFO] See broyden() in 'broyden.h' and 'broyden.cc'.\n"
              << "[INFO] The function follows the details in The Book (Chapters 9.3 and 10.2.3).\n";

    // ================================================================================
    // TASK 2 - Implementation of Quasi-Newton mininization with Symmetrized Broyden's update (Symmetrized Broyden)
    // ================================================================================
    section("TASK 2 - Implementation of Quasi-Newton mininization with Symmetrized Broyden's update (Symmetrized Broyden)");
    
    std::cout << "[INFO] See broyden_symmetrized() in 'broyden.h' and 'broyden.cc'.\n"
              << "[INFO] The function follows the details in The Book (Chapters 9.3 and 10.2.3).\n";

    // ================================================================================
    //                  PHASE 2: TEST ON FUNCTIONS WITH KNOWN MINIMA
    // ================================================================================

    // ================================================================================
    // TASK 3 - Tests on Rosenbrock Valley Function in 2D
    // ================================================================================
    section("3.1 - Tests on Rosenbrock Valley Function in 2D");
    
    // Efficient and numerically stable Rosenbrock function implementation
    auto f_rose_2d = [](const vector& vec) {
        double x = vec[0]; double y = vec[1];
        double term1 = 1.0 - x;
        double term2 = y - x * x;
        return (term1 * term1) + 100.0 * (term2 * term2);
    };
    
    // --------------------------------------------------------------------------------
    // 3.1 - BASIC TEST ON 2D ROSENBROCK
    // --------------------------------------------------------------------------------
    substep("3.1 - BASIC TEST ON 2D ROSENBROCK");
    
    vector x_known_base_rose_2d({1.0, 1.0});   // the Rosenbrock function has only one minimum
    vector x_start_base_rose_2d({-1.2, 1.0});  // initial point used for the system validation

    // Configuration print
    std::cout << "[INFO] Solver configuration:\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Initial point, x_start:        (" << x_start_base_rose_2d[0] << ", " << x_start_base_rose_2d[1] << ")\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Target tolerance, acc:         " << acc_tol << "\n";

    // Analytical minimum print
    std::cout << "[INFO] Known analytical minimum on 2D Rosenbrock:\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Known minimum point, x:        (" << x_known_base_rose_2d[0] << ", " << x_known_base_rose_2d[1] << ")\n";
    std::cout << "    Known minimum value, fx:       " << f_rose_2d(x_known_base_rose_2d) << "\n\n";

    // Standard Solver Verification Run
    broyden_log_filename = "path_rose_std_base.dat";
    std::cout << "[RUNNING] Standard Broyden Solver...\n";
    vector res_rose_2d_std_base = broyden(f_rose_2d, x_start_base_rose_2d, acc_tol);
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "--> Results:\n";
    std::cout << "    Found minimum point, x:        (" << res_rose_2d_std_base[0] << ", " << res_rose_2d_std_base[1] << ")\n";
    std::cout << "    Found minimum value, fx:       " << broyden_final_z << "\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Final gradient norm, ‖∇fx‖:    " << broyden_final_gnorm << "\n";
    std::cout << std::defaultfloat << "    Algorithm steps taken:         " << broyden_steps << "\n";
    std::cout << "    Convergence status:            " << format_status(broyden_final_gnorm < acc_tol, broyden_exit_reason) << "\n";
    std::cout << "    Log file written:              " << broyden_log_filename << "\n\n"; 
    broyden_log_filename = "";

    // Symmetrized Solver Verification Run
    broyden_log_filename = "path_rose_sym_base.dat";
    std::cout << "[RUNNING] Symmetrized Broyden Solver...\n";
    vector res_rose_2d_sym_base = broyden_symmetrized(f_rose_2d, x_start_base_rose_2d, acc_tol);
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "--> Results:\n";
    std::cout << "    Found minimum point, x:        (" << res_rose_2d_sym_base[0] << ", " << res_rose_2d_sym_base[1] << ")\n";
    std::cout << "    Found minimum value, fx:       " << broyden_final_z << "\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Final gradient norm, ‖∇fx‖:    " << broyden_final_gnorm << "\n";
    std::cout << std::defaultfloat << "    Algorithm steps taken:         " << broyden_steps << "\n";
    std::cout << "    Convergence status:            " << format_status(broyden_final_gnorm < acc_tol, broyden_exit_reason) << "\n";
    std::cout << "    Log file written:              " << broyden_log_filename << "\n\n"; 
    broyden_log_filename = "";

    std::cout << "[INFO] Plot script: See 3.1 in 'plot.gpi'.\n";
    std::cout << "[INFO] Plot: See 'rosenbrock_convergence_base.png'.\n\n";

    // --------------------------------------------------------------------------------
    // 3.2 - PERFORMANCE FROM SELECTED INITIAL POINTS
    // --------------------------------------------------------------------------------
    substep("3.2 - PERFORMANCE FROM SELECTED INITIAL POINTS");

    // Array containing variant target locations to stress-test topological features
    struct TestLocation { double x; double y; std::string file_tag; };
    TestLocation locations[] = {
        {-1.2,  1.0, "1"},  // (basic test) Sharp valley cornering and rapid direction changes.
        {-0.0, -0.0, "2"},  // Wall-crash recovery under sudden gradient shocks.
        {2.0,   2.0, "3"},  // High-velocity overshoots and flat-plateau braking.
        {99.0, 99.0, "4"}   // Long-range endurance against accumulated asymmetric drift.
    };

    // Configuration print
    std::cout << "[INFO] Solver configuration:\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Target tolerance, acc:         " << acc_tol << "\n";

    // Analytical minimum print
    std::cout << "[INFO] Known analytical minimum on 2D Rosenbrock:\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Known minimum point, x:        (" << x_known_base_rose_2d[0] << ", " << x_known_base_rose_2d[1] << ")\n";
    std::cout << "    Known minimum value, fx:       " << f_rose_2d(x_known_base_rose_2d) << "\n\n";

    // Note
    std::cout << "[INFO] Here I use 'std::chrono' to time the solvers. In Task 6 I will use GNU time.\n";
    std::cout << "[INFO] Step data is logged to 'path_rose_std_#.dat' with # = 1, 2, 3, 4.\n\n";

    // Table Header Printing
    std::cout << std::string(120, '=') << "\n";
    std::cout << std::left << std::setw(15) << "Initial point" 
              << std::setw(11) << "Algorithm"
              << std::setw(14) << "Final fx"
              << std::setw(14) << "Final ‖∇fx‖"  
              << std::setw(7)  << "   Steps" 
              << std::setw(14) << "  Time (s)" 
              << "  Convergence status\n";
    std::cout << std::string(120, '=') << "\n";

    for (size_t i = 0; i < 4; ++i) {
        const auto& loc = locations[i];
        vector x_start({loc.x, loc.y});
        
        // Format string for the coordinate display box
        std::string coord_str = "[" + std::to_string(loc.x).substr(0,4) + ", " + std::to_string(loc.y).substr(0,4) + "]";

        // RUN 1: Standard Broyden
        // -----------------------
        broyden_log_filename = "path_rose_std_" + loc.file_tag + ".dat";
        
        auto t0_std = std::chrono::high_resolution_clock::now(); // use std::chrono to measure time
        vector res_std = broyden(f_rose_2d, x_start, acc_tol);
        auto t1_std = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dt_std = t1_std - t0_std;
        
        // Instantly snapshot global state values before the symmetric run wipes them
        double z_std     = broyden_final_z;
        int steps_std    = broyden_steps;
        double gnorm_std = broyden_final_gnorm;
        std::string stat_std = format_status(gnorm_std < acc_tol, broyden_exit_reason);

        // RUN 2: Symmetrized Broyden
        // --------------------------
        broyden_log_filename = "path_rose_sym_" + loc.file_tag + ".dat";
        
        auto t0_sym = std::chrono::high_resolution_clock::now();
        vector res_sym = broyden_symmetrized(f_rose_2d, x_start, acc_tol);
        auto t1_sym = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dt_sym = t1_sym - t0_sym;
        
        double z_sym     = broyden_final_z;
        int steps_sym    = broyden_steps;
        double gnorm_sym = broyden_final_gnorm;
        std::string stat_sym = format_status(gnorm_sym < acc_tol, broyden_exit_reason);

        // PRINTING ROW PAIR
        // -----------------
        // Row A: Standard Broyden
        std::cout << std::left  << std::setw(15) << coord_str
                  << std::setw(11) << "Standard"
                  << std::scientific << std::setprecision(6)
                  << std::setw(14) << dt_std.count()
                  << std::setw(14) << z_std
                  << std::defaultfloat << std::setw(7) << steps_std
                  << std::scientific << std::setprecision(4)
                  << std::setw(14) << gnorm_std
                  << stat_std << "\n";

        // Row B: Symmetrized Broyden (Leaves the coordinate space blank for visual grouping)
        std::cout << std::left  << std::setw(15) << ""
                  << std::setw(11) << "Symmetric"
                  << std::scientific << std::setprecision(6)
                  << std::setw(14) << dt_sym.count()
                  << std::setw(14) << z_sym
                  << std::defaultfloat << std::setw(7) << steps_sym
                  << std::scientific << std::setprecision(4)
                  << std::setw(14) << gnorm_sym
                  << stat_sym << "\n";

        // Draw structural separator between distinct geographical points
        if (i < 3) {
            std::cout << std::string(120, '-') << "\n";
        }
    }
    std::cout << std::string(120, '=') << "\n\n";
    broyden_log_filename = ""; // Secure global resetting

    // --------------------------------------------------------------------------------
    // 3.3 - ROSENBROCK CONVERGENCE EFFICIENCY GRID SWEEP
    // -------------------------------------------------------------------------------
    substep("3.3 - ROSENBROCK CONVERGENCE EFFICIENCY GRID SWEEP");

    std::cout << "[RUNNING] Mapping Rosenbrock Convergence Efficiency Grid (Std vs Sym)..." << std::endl;

    // Disable per-step path text logging for performance
    broyden_log_filename = ""; 

    // Open dedicated basin log file
    std::ofstream rosen_basin_file("rosen_basin.dat");
    rosen_basin_file << "# Rosenbrock basin log\n";
    rosen_basin_file << "# X Y Tier_Std Tier_Sym\n";

    // Open dedicated failure log file
    std::ofstream failure_file("rosen_failures.dat");
    failure_file << "# Rosenbrock optimization failure log\n";
    failure_file << "# Init_X      Init_Y      Final_X     Final_Y     Solver_Type\n";

    // Grid parameters matching contour boundaries
    int r_grid_points = 100;
    double min_x = -2.0, max_x = 2.0;
    double min_y = -1.0, max_y = 3.0;
    
    double step_x = (max_x - min_x) / (r_grid_points - 1);
    double step_y = (max_y - min_y) / (r_grid_points - 1);

    // High-resolution performance classification trackers
    std::vector<int> stats_rose_std(6, 0);
    std::vector<int> stats_rose_sym(6, 0);

    auto classify_performance = [&](const vector& res) -> int {
        // Check global convergence
        if (broyden_final_gnorm >= acc_tol) return 0; // Failed (Gradient check)
        
        double dx = res[0] - 1.0; double dy = res[1] - 1.0;
        if (std::sqrt(dx*dx + dy*dy) > coord_tol) return 0;   // Failed (Coordinate check)

        if (broyden_steps <= 15)               return 1; // Tier 1: Efficient
        if (broyden_steps <= 30)               return 2; // Tier 2: Fast
        if (broyden_steps <= 50)               return 3; // Tier 3: Medium
        if (broyden_steps <= 80)               return 4; // Tier 4: Slow
        return 5;                                        // Tier 5: Tortuous path
    };

    // Force strict fixed-point precision formatting for the text streams
    rosen_basin_file << std::scientific << std::setprecision(6);
    failure_file << std::scientific << std::setprecision(6);

    // Execute the unified spatial sweep
    for (int i = 0; i < r_grid_points; ++i) {
        double x_start = min_x + i * step_x;
        for (int j = 0; j < r_grid_points; ++j) {
            double y_start = min_y + j * step_y;
            vector trial_start({x_start, y_start});
            
            // Evaluate Standard Broyden
            vector res_std = broyden(f_rose_2d, trial_start, acc_tol);
            int perf_std = classify_performance(res_std);
            stats_rose_std[perf_std]++;
            
            // Log Standard failures if Tier 0 is flagged
            if (perf_std == 0) {
                failure_file << x_start << " " << y_start << " "
                             << res_std[0] << " " << res_std[1] << " Standard\n";
            }

            // Evaluate Symmetrized Broyden
            vector res_sym = broyden_symmetrized(f_rose_2d, trial_start, acc_tol);
            int perf_sym = classify_performance(res_sym);
            stats_rose_sym[perf_sym]++;

            // Log Symmetrized failures if Tier 0 is flagged
            if (perf_sym == 0) {
                failure_file << x_start << " " << y_start << " "
                             << res_sym[0] << " " << res_sym[1] << " Symmetrized\n";
            }
            
            // Log coordinates and both performance profiles side-by-side
            rosen_basin_file << x_start << " " << y_start << " " << perf_std << " " << perf_sym << "\n";
        }
        rosen_basin_file << "\n"; // Keeps gnuplot matrix grouping happy
    }
    rosen_basin_file.close();
    failure_file.close();

    std::cout << "--> Rosenbrock basin mapped successfully.\n";
    std::cout << "[INFO] Basin mapping data saved to 'rosen_basin.dat'.\n";
    std::cout << "[INFO] Failure data saved to 'himmel_failures.dat'.\n\n";

    std::cout << "[INFO] Plot script: See 3.3 in 'plot.gpi'.\n";
    std::cout << "[INFO] Plot: See 'rosenbrock_basin.svg'.\n\n";

    // Summary Reports
    std::cout << "==================================================\n";
    std::cout << "ROSENBROCK SWEEP SUMMARY (" << r_grid_points * r_grid_points << " points total)\n";
    std::cout << "==================================================\n";
    std::cout << "Performance Tier (ID)  | Standard  | Symmetrized\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Failed       (0)       | " << stats_rose_std[0] << "       | " << stats_rose_sym[0] << "\n";
    std::cout << "≤ 15  steps  (1)       | " << stats_rose_std[1] << "       | " << stats_rose_sym[1] << "\n";
    std::cout << "16-30 steps  (2)       | " << stats_rose_std[2] << "      | " << stats_rose_sym[2] << "\n";
    std::cout << "31-50 steps  (3)       | " << stats_rose_std[3] << "      | " << stats_rose_sym[3] << "\n";
    std::cout << "51-80 steps  (4)       | " << stats_rose_std[4] << "      | " << stats_rose_sym[4] << "\n";
    std::cout << "80 <  steps  (5)       | " << stats_rose_std[5] << "        | " << stats_rose_sym[5] << "\n";
    std::cout << "==================================================\n\n";


    // ================================================================================
    // TASK 4 - Tests on Hummelblau Function
    // ================================================================================
    substep("TASK 4 - Tests on Hummelblau Function");

    // Efficient and numerically stable Himmelblau's function implementation
    auto f_himmel = [](const vector& vec) {
        double t1 = vec[0]*vec[0] + vec[1] - 11.0;
        double t2 = vec[0] + vec[1]*vec[1] - 7.0;
        return t1*t1 + t2*t2;
    };

    // --------------------------------------------------------------------------------
    // 4.1 - BASIC TEST ON HIMMELBLAU
    // --------------------------------------------------------------------------------
    substep("4.1 - BASIC TEST ON HIMMELBLAU");
    
    vector x_expect_base_himmel({3.0, 2.0});   // the expected minimum given the initial point
    vector x_start_base_himmel({1.0, 1.0});    // initial point used for the system validation

    // Configuration print
    std::cout << "[INFO] Solver configuration:\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Initial point, x_start:        (" << x_start_base_himmel[0] << ", " << x_start_base_himmel[1] << ")\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Target tolerance, acc:         " << acc_tol << "\n";

    // Analytical minimum print
    std::cout << "[INFO] Expected analytical minimum on Himmelblau given initial point:\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Expected minimum point, x:     (" << x_expect_base_himmel[0] << ", " << x_expect_base_himmel[1] << ")\n";
    std::cout << "    Expected minimum value, fx:    " << f_himmel(x_expect_base_himmel) << "\n\n";

    // Standard Solver Verification Run
    broyden_log_filename = "";
    std::cout << "[RUNNING] Standard Broyden Solver...\n";
    vector res_himmel_std_base = broyden(f_himmel, x_start_base_himmel, acc_tol);
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "--> Results:\n";
    std::cout << "    Found minimum point, x:        (" << res_himmel_std_base[0] << ", " << res_himmel_std_base[1] << ")\n";
    std::cout << "    Found minimum value, fx:       " << broyden_final_z << "\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Final gradient norm, ‖∇fx‖:    " << broyden_final_gnorm << "\n";
    std::cout << std::defaultfloat << "    Algorithm steps taken:         " << broyden_steps << "\n";
    std::cout << "    Convergence status:            " << format_status(broyden_final_gnorm < acc_tol, broyden_exit_reason) << "\n\n";

    // Symmetrized Solver Verification Run
    broyden_log_filename = "";
    std::cout << "[RUNNING] Symmetrized Broyden Solver...\n";
    vector res_himmel_sym_base = broyden_symmetrized(f_himmel, x_start_base_himmel, acc_tol);
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "--> Results:\n";
    std::cout << "    Found minimum point, x:        (" << res_himmel_sym_base[0] << ", " << res_himmel_sym_base[1] << ")\n";
    std::cout << "    Found minimum value, fx:       " << broyden_final_z << "\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Final gradient norm, ‖∇fx‖:    " << broyden_final_gnorm << "\n";
    std::cout << std::defaultfloat << "    Algorithm steps taken:         " << broyden_steps << "\n";
    std::cout << "    Convergence status:            " << format_status(broyden_final_gnorm < acc_tol, broyden_exit_reason) << "\n\n";

    // --------------------------------------------------------------------------------
    // 4.2 - PERFORMANCE FROM SELECTED INITIAL POINTS
    // --------------------------------------------------------------------------------
    substep("4.2 - PERFORMANCE FROM SELECTED INITIAL POINTS");

    // Array of five distinct starting coordinates to capture all four valleys
    std::vector<vector> start_himmel = {
        vector({ 1.0,  1.0}),            // (basic test) expected to converge to (3.0, 2.0)
        vector({-0.270845, -0.923039}),  // Close to local maximum
        vector({ 0.0,  0.0}),            // Central between the four basins
        vector({ 0.08668, 2.88425}),     // Near saddle point
        vector({99.0, 99.0})             // Far away
    };

    // Analytical global minima locations
    std::vector<vector> expec_himmel = {
        vector({ 3.0,                  2.0}),
        vector({ 3.58442834033044959, -1.84812652696539953}),
        vector({-2.80511808695282632,  3.13131251825132986}),
        vector({-3.77931025337839355, -3.28318599128558153})
    };

    // Configuration print
    std::cout << "[INFO] Solver configuration:\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Target tolerance, acc:         " << acc_tol << "\n\n";

    std::cout << "[INFO] Analytical global minima on Himmelblau:\n";
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Point in quadrant (+,+):       (" << expec_himmel[0][0] << ", " << expec_himmel[0][1] << ")\n";
    std::cout << "    Point in quadrant (-,+):       (" << expec_himmel[1][0] << ", " << expec_himmel[1][1] << ")\n";
    std::cout << "    Point in quadrant (-,-):       (" << expec_himmel[2][0] << ", " << expec_himmel[2][1] << ")\n";
    std::cout << "    Point in quadrant (+,-):       (" << expec_himmel[3][0] << ", " << expec_himmel[3][1] << ")\n";
    std::cout << "    Function value (in all):       " << f_himmel(x_expect_base_himmel) << "\n\n";
    std::cout << std::scientific << std::setprecision(4);

    std::cout << "[INFO] Here I use 'std::chrono' to time the solvers. In PHASE 5 I will use GNU time.\n";
    std::cout << "[INFO] Step data is logged to 'path_himmel_std_#.dat' with # = 1, 2, 3, 4.\n\n";

    // Table Header Printing
    std::cout << std::string(120, '=') << "\n";
    std::cout << std::left << std::setw(15) << "Initial point" 
              << std::setw(11) << "Algorithm"
              << std::setw(14) << "Final fx"
              << std::setw(14) << "Final ‖∇fx‖"  
              << std::setw(7)  << "   Steps" 
              << std::setw(14) << "  Time (s)" 
              << "  Convergence status\n";
    std::cout << std::string(120, '=') << "\n";

    for (size_t i = 0; i < start_himmel.size(); ++i) {
        // Construct standard localized boundary tokens dynamically via clean stream states
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << "[" << start_himmel[i][0] << ", " << start_himmel[i][1] << "]";
        std::string coord_str = ss.str();

        // Run 1: Standard Broyden
        // -----------------------
        broyden_log_filename = "path_himmel_std_" + std::to_string(i + 1) + ".dat";
        
        auto t0_std = std::chrono::high_resolution_clock::now();
        vector res_std = broyden(f_himmel, start_himmel[i], acc_tol);
        auto t1_std = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dt_std = t1_std - t0_std;
        
        // Secure execution cache boundary snapshotting
        double z_std         = broyden_final_z;
        int steps_std        = broyden_steps;
        double gnorm_std     = broyden_final_gnorm;
        std::string stat_std = format_status(gnorm_std < acc_tol, broyden_exit_reason);

        // Run 2: Symmetrized Broyden
        // --------------------------
        broyden_log_filename = "path_himmel_sym_" + std::to_string(i + 1) + ".dat";
        
        auto t0_sym = std::chrono::high_resolution_clock::now();
        vector res_sym = broyden_symmetrized(f_himmel, start_himmel[i], acc_tol);
        auto t1_sym = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dt_sym = t1_sym - t0_sym;
        
        // Secure secondary cache pipeline boundary
        double z_sym         = broyden_final_z;
        int steps_sym        = broyden_steps;
        double gnorm_sym     = broyden_final_gnorm;
        std::string stat_sym = format_status(gnorm_sym < acc_tol, broyden_exit_reason);

        // PRINTING ROW PAIR
        // -----------------
        // Row A: Standard Solver Metrics
        std::cout << std::left  << std::setw(15) << coord_str
                  << std::setw(11) << "Standard"
                  << std::scientific << std::setprecision(6)
                  << std::setw(14) << dt_std.count()
                  << std::setw(14) << z_std
                  << std::defaultfloat << std::setw(7) << steps_std
                  << std::scientific << std::setprecision(4)
                  << std::setw(14) << gnorm_std
                  << stat_std << "\n";

        // Row B: Symmetrized Solver Metrics
        std::cout << std::left  << std::setw(15) << ""
                  << std::setw(11) << "Symmetric"
                  << std::scientific << std::setprecision(6)
                  << std::setw(14) << dt_sym.count()
                  << std::setw(14) << z_sym
                  << std::defaultfloat << std::setw(7) << steps_sym
                  << std::scientific << std::setprecision(4)
                  << std::setw(14) << gnorm_sym
                  << stat_sym << "\n";

        if (i < start_himmel.size() - 1) {
            std::cout << std::string(120, '-') << "\n";
        }
    }
    std::cout << std::string(120, '=') << "\n\n";
    broyden_log_filename = ""; // Secure global wiping

    std::cout << "[INFO] Plot script: See 4.2 in 'plot.gpi'.\n";
    std::cout << "[INFO] Plot: See 'himmelblau_convergence.svg'.\n\n";

    // --------------------------------------------------------------------------------
    // 4.3 - HIMMELBLAU BASINS OF ATTRACTION GRID SWEEP
    // --------------------------------------------------------------------------------
    substep("4.3 - HIMMELBLAU BASINS OF ATTRACTION GRID SWEEP");

    // Analytical Gradient of the Himmelblau Function
    auto grad_himmel = [](double x, double y) -> std::pair<double, double> {
        double dfdx = 4.0 * x * (x * x + y - 11.0) + 2.0 * (x + y * y - 7.0);
        double dfdy = 2.0 * (x * x + y - 11.0) + 4.0 * y * (x + y * y - 7.0);
        return {dfdx, dfdy};
    };

    // Simulator to find the geometrically expected physical basin (1, 2, 3, or 4)
    auto get_true_basin = [&](double x0, double y0) -> int {
        double x = x0;
        double y = y0;
        double alpha = 0.002; // Small, safe step size to ensure no ridge jumping
        int max_steps = 1500;
        
        for (int k = 0; k < max_steps; ++k) {
            auto [dfdx, dfdy] = grad_himmel(x, y);
            if (std::hypot(dfdx, dfdy) < coord_tol) break; // Stopped at a minimum
            
            x -= alpha * dfdx; // Roll downhill in the direction of the gradient
            y -= alpha * dfdy;
        }
        
        // Match the resting coordinate to the known analytical roots
        for (size_t id = 0; id < expec_himmel.size(); ++id) {
            double dx = x - expec_himmel[id][0];
            double dy = y - expec_himmel[id][1];
            if (std::hypot(dx, dy) < coord_tol) {
                return static_cast<int>(id + 1); // Returns 1, 2, 3, or 4
            }
        }
        return 0; // Hit the exact central peak or a strict saddle point ridge
    };


    std::cout << "[RUNNING] Mapping Himmelblau Basins of Attraction Grid Sweep (Std vs Sym)..." << std::endl;

    // Disable individual path logging to keep file I/O blazing fast
    broyden_log_filename = "";
    
    // Open dedicated basin log file
    std::ofstream basin_file_himmel("himmel_basins.dat");
    basin_file_himmel << "# Himmelblau basins log\n";
    basin_file_himmel << "# X Y Root_Std Root_Sym\n";
    
    // Open dedicated failure log file
    std::ofstream failure_file_himmel("himmel_failures.dat");
    failure_file_himmel << "# Himmelblau optimization failure log\n";
    failure_file_himmel << "# Init_X      Init_Y      Final_X     Final_Y     Solver_Type\n";

    // Grid parameters: 100x100 high-resolution resolution over the [-4.5, 4.5] domain
    int grid_points_himmel = 100;
    double min_val = -4.5;
    double max_val = 4.5;
    double step = (max_val - min_val) / (grid_points_himmel - 1);

    // Dynamic statistical counters for both mathematical engines
    std::vector<int> stats_himmel_std(5, 0);
    std::vector<int> stats_himmel_sym(5, 0);

    // Helper lambda to identify which unique root was captured (Reads global gnorm state)
    auto classify_root = [&](const vector& found) -> int {
        if (broyden_final_gnorm >= acc_tol) return 0; // Failed / Stalled

        // Check proximity to the 4 known roots using Euclidean distance
        for (size_t id = 0; id < expec_himmel.size(); ++id) {
            double dx = found[0] - expec_himmel[id][0];
            double dy = found[1] - expec_himmel[id][1];
            if (std::hypot(dx, dy) < coord_tol) {
                return static_cast<int>(id + 1); // Returns 1, 2, 3, or 4
            }
        }
        return 0; // Saddle points and maxima also counts as tier 0 failures
    };

    // Force strict fixed-point precision formatting for the text streams
    basin_file_himmel << std::scientific << std::setprecision(6);
    failure_file_himmel << std::scientific << std::setprecision(6);

    // Trackers for Basin Fidelity (How often did they land in the expected basin?)
    int correct_counts_std = 0;
    int correct_counts_sym = 0;
    int total_valid_points = 0;

    // Execute the unified 2D spatial grid sweep
    for (int i = 0; i < grid_points_himmel; ++i) {
        double x_start = min_val + i * step;
        for (int j = 0; j < grid_points_himmel; ++j) {
            double y_start = min_val + j * step;
            vector trial_start({x_start, y_start});

            // Get the true geometric baseline
            int true_basin = get_true_basin(x_start, y_start);
            
            // Evaluate Standard Broyden
            vector res_std = broyden(f_himmel, trial_start, acc_tol);
            int root_std = classify_root(res_std);
            stats_himmel_std[root_std]++;
            
            // Log Standard failures if Root 0 is flagged
            if (root_std == 0) {
                failure_file_himmel << x_start << " " << y_start << " "
                                    << res_std[0] << " " << res_std[1] << " Standard\n";
            }
            
            // Evaluate Symmetrized Broyden
            vector res_sym = broyden_symmetrized(f_himmel, trial_start, acc_tol);
            int root_sym = classify_root(res_sym);
            stats_himmel_sym[root_sym]++;
            
            // Log Symmetrized failures if Root 0 is flagged
            if (root_sym == 0) {
                failure_file_himmel << x_start << " " << y_start << " "
                                    << res_sym[0] << " " << res_sym[1] << " Symmetrized\n";
            }

            // Comparison Evaluation
            if (true_basin != 0) { // Ignore the exact unstable saddle points
                total_valid_points++;
                if (root_std == true_basin) correct_counts_std++;
                if (root_sym == true_basin) correct_counts_sym++;
            }
            
            // Log coordinates and both root classifications side-by-side
            basin_file_himmel << x_start << " " << y_start << " " << root_std << " " << root_sym << " " << true_basin << "\n";
        }
        basin_file_himmel << "\n"; // Blank line preserves gnuplot grid blocks
    }
    basin_file_himmel.close();
    failure_file_himmel.close();

    double fidelity_std = (static_cast<double>(correct_counts_std) / total_valid_points) * 100.0;
    double fidelity_sym = (static_cast<double>(correct_counts_sym) / total_valid_points) * 100.0;
    
    std::cout << " --> Himmelblau basins mapped successfully.\n";
    std::cout << "[INFO] Mapping data saved to 'himmel_basins.dat'.\n";
    std::cout << "[INFO] Failure data saved to 'himmel_failures.dat'.\n\n";

    std::cout << "[INFO] Plot script: See 4.3 in 'plot.gpi'.\n";
    std::cout << "[INFO] Plot: See 'himmelblau_basins.svg'.\n\n";

    // Summary Reports
    std::cout << "==================================================\n";
    std::cout << "HIMMELBLAU SWEEP SUMMARY (" << grid_points_himmel * grid_points_himmel << " points total)\n";
    std::cout << "==================================================\n";
    std::cout << "Method Outcome (ID)    | Standard    | Symmetrized\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Failed           (0)   | " << stats_himmel_std[0] << "           | " << stats_himmel_sym[0] << "\n";
    std::cout << "Root 1           (1)   | " << stats_himmel_std[1] << "        | " << stats_himmel_sym[1] << "\n";
    std::cout << "Root 2           (2)   | " << stats_himmel_std[2] << "        | " << stats_himmel_sym[2] << "\n";
    std::cout << "Root 3           (3)   | " << stats_himmel_std[3] << "        | " << stats_himmel_sym[3] << "\n";
    std::cout << "Root 4           (4)   | " << stats_himmel_std[4] << "        | " << stats_himmel_sym[4] << "\n";
    std::cout << "---------------------------------------------------\n";
    std::cout << "Fidelity Score           " << std::fixed << std::setprecision(2) << fidelity_std << "%        " << fidelity_sym << "%\n";
    std::cout << "===================================================\n\n";

    std::cout << "[INFO] The fidelity score measures the % of initial points that converged to\ntheir mathematically expected (steepest descent) natural geometric basin.\n";

    
    // ================================================================================
    //                  PHASE 3: APPLICATION TO A MORE COMPLICATED PROBLEM
    // ================================================================================

    // ================================================================================
    // TASK 5 - Higgs parameter optimization
    // ================================================================================
    section("TASK 5 -  Higgs parameter optimization");
    std::cout << "[INFO] This task is inspired by Task B in homework 21_minimum.\n";

    // --------------------------------------------------------------------------------
    // 5.1 - PREPARATION OF HIGGS DATA FROM FILE
    // --------------------------------------------------------------------------------
    substep("5.1 - PREPARATION OF HIGGS DATA FROM FILE");

    // Open the data file directly using a file stream
    std::string filename = "higgs.data.txt";
    std::ifstream data_file(filename);

    // Read formatted data table line-by-line from standard input
    std::vector<double> energy, signal, error;
    std::string line;
    double energy_val, signal_val, error_val;

    while (std::getline(data_file, line)) {
        // Skip empty lines or comment lines starting with '#'
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::stringstream ss(line);
        if (ss >> energy_val >> signal_val >> error_val) {
            energy.push_back(energy_val);
            signal.push_back(signal_val);
            error.push_back(error_val);
        }
    }
    data_file.close(); // Clean up resource
    std::cout << "[INFO] Successfully read " << energy.size() << " data points in 'higgs.data.txt' from the standard input.\n";

    // Define the Breit-Wigner fitting deviation function
    // v[0] = mass (m), v[1] = width (Gamma), v[2] = scale factor (A)
    auto higgs_deviation = [&](const vector& v) {
        double m     = v[0];
        double gamma = v[1];
        double A     = v[2];
        
        double sum_sq_dev = 0.0;
        for (size_t i = 0; i < energy.size(); ++i) {
            double E = energy[i];
            // Breit-Wigner function F(E | m, gamma, A)
            double F = A / (std::pow(E - m, 2) + std::pow(gamma, 2) / 4.0);
            
            double deviation = (F - signal[i]) / error[i];
            sum_sq_dev += deviation * deviation;
        }
        return sum_sq_dev;
    };
    std::cout << "[INFO] Successfully defined the deviation function: D(m,Γ,A)=Σi[(F(Ei|m,Γ,A)-σi)/Δσi]^2\n\n";

    // --------------------------------------------------------------------------------
    // 5.2 - FIT OF BREIT-WIGNER TO HIGGS DATA
    // --------------------------------------------------------------------------------
    substep("5.2 - FIT OF BREIT-WIGNER TO HIGGS DATA");
    
    // Initial guess
    vector start_higgs({125.0, 2.0, 10.0}); 
    size_t higgs_dim = start_higgs.size();
    
    std::cout << "[INFO] Solver Configuration:\n"
              << "    Initial mass (m):              " << start_higgs[0] << " GeV/c^2\n"
              << "    Initial width (Γ):             " << start_higgs[1] << " GeV\n"
              << "    Initial scale (A):             " << start_higgs[2] << "\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "    Target tolerance, acc:         " << acc_tol << "\n\n";

    // Reusable function to extract fitted parameter errors using a deterministic engine
    auto compute_uncertainties = [&](const vector& opt_v, double opt_f) {
        vector g_opt = gradient(higgs_deviation, opt_v, opt_f);
        matrix H     = hessian(higgs_deviation, opt_v, g_opt);
        vector unc(higgs_dim);
        
        // Inline deterministic linear solver (Gaussian Elimination with Partial Pivoting)
        auto solve_linear_system = [&](matrix A, vector b) {
            size_t n = b.size();
            
            // Forward Elimination
            for (size_t i = 0; i < n; ++i) {
                // Partial Pivoting: Find the largest element in column i to maintain numerical stability
                size_t max_row = i;
                for (size_t k = i + 1; k < n; ++k) {
                    if (std::abs(A(k, i)) > std::abs(A(max_row, i))) {
                        max_row = k;
                    }
                }
                // Swap rows if necessary
                if (max_row != i) {
                    for (size_t k = i; k < n; ++k) std::swap(A(i, k), A(max_row, k));
                    std::swap(b[i], b[max_row]);
                }
                
                // Eliminate values below the pivot
                for (size_t k = i + 1; k < n; ++k) {
                    double factor = A(k, i) / A(i, i);
                    for (size_t j = i; j < n; ++j) {
                        A(k, j) -= factor * A(i, j);
                    }
                    b[k] -= factor * b[i];
                }
            }
            
            // Back Substitution
            vector x(n);
            for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
                double sum = 0.0;
                for (size_t j = i + 1; j < n; ++j) {
                    sum += A(i, j) * x[j];
                }
                x[i] = (b[i] - sum) / A(i, i);
            }
            return x;
        };

        // Evaluate standard errors column-by-column
        for (size_t j = 0; j < higgs_dim; ++j) {
            vector b(higgs_dim);
            b[j] = 2.0; // Target vector scaled by 2 for covariance mapping: 2 * e_j
            
            // Deterministically solve H * c = 2*e_j
            vector solved_col = solve_linear_system(H, b);
            
            // Parameter uncertainty is the square root of the diagonal element Cov(j,j)
            // std::max guards against microscopic negative floating-point noise near zero
            unc[j] = std::sqrt(std::max(0.0, solved_col[j]));
        }
        
        return unc;
    };

    // STANDARD BROYDEN
    // ----------------
    broyden_log_filename = "path_higgs_std.dat";
    std::cout << "[RUNNING] Standard Broyden Solver...\n";
    vector res_higgs_std = broyden(higgs_deviation, start_higgs, acc_tol);
    vector unc_std = compute_uncertainties(res_higgs_std, broyden_final_z);
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "--> Results:\n";
    std::cout << "    Mass (m):                      " << res_higgs_std[0] << " ± " << unc_std[0] << "\n";
    std::cout << "    Width (Γ):                     " << res_higgs_std[1] << " ± " << unc_std[1] << "\n";
    std::cout << "    Scale (A):                     " << res_higgs_std[2] << " ± " << unc_std[2] << "\n";
    std::cout << "    Final minimized D(m,Γ,A):      " << broyden_final_z << "\n";
    std::cout << std::defaultfloat << "    Algorithm steps taken:         " << broyden_steps << "\n";
    std::cout << "    Convergence status:            " << format_status(broyden_final_gnorm < acc_tol, broyden_exit_reason) << "\n";
    std::cout << "    Log file written:              " << broyden_log_filename << "\n\n"; 
    broyden_log_filename = "";

    // SYMMETRIZED BROYDEN
    // -------------------
    broyden_log_filename = "path_higgs_sym.dat";
    std::cout << "[RUNNING] Symmetrized Broyden Solver...\n";
    vector res_higgs_sym = broyden_symmetrized(higgs_deviation, start_higgs, acc_tol);
    vector unc_sym = compute_uncertainties(res_higgs_sym, broyden_final_z);
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "--> Results:\n";
    std::cout << "    Mass (m):                      " << res_higgs_sym[0] << " ± " << unc_sym[0] << "\n";
    std::cout << "    Width (Γ):                     " << res_higgs_sym[1] << " ± " << unc_sym[1] << "\n";
    std::cout << "    Scale (A):                     " << res_higgs_sym[2] << " ± " << unc_sym[2] << "\n";
    std::cout << "    Final minimized D(m,Γ,A):      " << broyden_final_z << "\n";
    std::cout << std::defaultfloat << "    Algorithm steps taken:         " << broyden_steps << "\n";
    std::cout << "    Convergence status:            " << format_status(broyden_final_gnorm < acc_tol, broyden_exit_reason) << "\n";
    std::cout << "    Log file written:              " << broyden_log_filename << "\n\n"; 
    broyden_log_filename = "";

    // --------------------------------------------------------------------------------
    // 5.3 - PLOT OF FIT CURVES
    // --------------------------------------------------------------------------------
    substep("5.3 - PLOT OF FIT CURVES");

    // Reusable lambda to generate and save Breit-Wigner curve data
    auto save_fit_curve = [&](const std::string& output_name, const vector& res) {
        std::ofstream fit_file(output_name);
        if (!fit_file.is_open()) return;
        
        double opt_m     = res[0];
        double opt_gamma = res[1];
        double opt_A     = res[2];

        for (double E = 100.0; E <= 160.0; E += 0.05) {
            double F = opt_A / (std::pow(E - opt_m, 2) + (opt_gamma * opt_gamma) / 4.0);
            fit_file << E << " " << F << "\n";
        }
        fit_file.close();
    };

    // Export the individual curves
    save_fit_curve("higgs_fit_std.dat", res_higgs_std);
    std::cout << "[INFO] Breit-Wigner curve generated for standard solver fit:  'higgs_fit_std.dat'" << "\n";    

    save_fit_curve("higgs_fit_sym.dat", res_higgs_sym);
    std::cout << "[INFO] Breit-Wigner curve generated for symmetrized solver fit:  'higgs_fit_sym.dat'" << "\n";    

    // Save the raw experimental data to an explicit data file for plotting
    std::ofstream exp_file("higgs_data_clean.dat");
    if (exp_file.is_open()) {
        for (size_t i = 0; i < energy.size(); ++i) {
            exp_file << energy[i] << " " << signal[i] << " " << error[i] << "\n";
        }
        exp_file.close();
    }

    std::cout << "[INFO] Experimental data saved to:  'higgs_data_clean.dat'\n\n";
    std::cout << "[INFO] Plot script: See 5.3 in 'plot.gpi'.\n";
    std::cout << "[INFO] Plot: See 'higgs_fit.svg'.\n\n";

    // --------------------------------------------------------------------------------
    // 5.4 - PLOT OF CONVERGENCE HISTORY
    // --------------------------------------------------------------------------------
    substep("5.4 - PLOT OF CONVERGENCE HISTORY");

    // Keep lambda that safely parses space-separated 3D coordinates
    auto load_log_history = [](const std::string& path_to_log) {
        std::vector<std::vector<double>> history;
        std::ifstream file(path_to_log);
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open log file: " << path_to_log << "\n";
            return history;
        }
        
        double m = 0.0, g = 0.0, A = 0.0;
        while (file >> m >> g >> A) {
            history.push_back({m, g, A});
        }
        return history;
    };

    // Insert the new exporter lambda right underneath it
    auto save_deviation_history = [&](const std::string& filename_higgs, const std::vector<std::vector<double>>& history) {
        std::ofstream file(filename_higgs);
        if (!file.is_open()) return;
        file << "# Iteration Deviation_D\n";
        for (size_t i = 0; i < history.size(); ++i) {
            vector coords({history[i][0], history[i][1], history[i][2]});
            file << i << " " << higgs_deviation(coords) << "\n";
        }
        file.close();
    };

    // Load the raw step histories using the parser
    auto history_std = load_log_history("path_higgs_std.dat");
    auto history_sym = load_log_history("path_higgs_sym.dat");

    // Compute deviations and write them out cleanly
    save_deviation_history("higgs_dev_std.dat", history_std);
    save_deviation_history("higgs_dev_sym.dat", history_sym);

    std::cout << "[INFO] Convergence history data (standard solver):  'higgs_dev_std.dat'\n";
    std::cout << "[INFO] Convergence history data (symmetrized solver):  'higgs_dev_sym.dat'\n\n";
    std::cout << "[INFO] Plot script: See 5.4 in 'plot.gpi'.\n";
    std::cout << "[INFO] Plot: See 'higgs_convergence.svg'.\n";


    // ================================================================================
    //                         PHASE 4: DIMENSIONAL BENCHMARKING
    // ================================================================================

    // ================================================================================
    // TASK 6 - Dimensional Benchmarking on the Generalized Rosenbrock Function
    // ================================================================================
    section("TASK 6 - Dimensional Benchmarking on the Generalized Rosenbrock Function");
   
    // --------------------------------------------------------------------------------
    // 6.1 - DIMENSIONAL SCALING PERFORMANCE
    // --------------------------------------------------------------------------------
    substep("6.1 - DIMENSIONAL SCALING PERFORMANCE");

    std::cout << "[INFO] The Generalized Rosenbrock function is implemented in 'rose_benchmark.h'.\n\n";    

    std::cout << std::string(70, '=') << "\n";
    std::cout << std::left << std::setw(8) << "Dim(N)" 
                << std::setw(12) << "Method" 
                << std::setw(20) << "Function Calls" 
                << std::setw(12) << "Steps" 
                << std::setw(18) << "Target Deviation" << "\n";
    std::cout << std::string(70, '=') << "\n";

    for (size_t dim : {2, 4, 8, 16, 32}) {
        vector n_start(dim);
        for (size_t i = 0; i < dim; ++i) {
            n_start[i] = (i % 2 == 0) ? -1.2 : 1.0;
        }

        // --- Standard Broyden ---
        int calls_std = 0;
        broyden_steps = 0; 
        auto f_std = [&calls_std](const vector& v) { 
            return RosenbrockBenchmarker::rosenbrockNd(v, calls_std); 
        };
        vector res_std = broyden(f_std, n_start, acc_tol); //

        std::cout << std::left << std::setw(8) << dim 
                    << std::setw(12) << "Standard" 
                    << std::setw(20) << calls_std 
                    << std::setw(12) << broyden_steps 
                    << std::scientific << std::setprecision(4) << std::setw(18) << RosenbrockBenchmarker::calculateDeviation(res_std) << "\n";

        // --- Symmetrized Broyden ---
        int calls_sym = 0;
        broyden_steps = 0; 
        auto f_sym = [&calls_sym](const vector& v) { 
            return RosenbrockBenchmarker::rosenbrockNd(v, calls_sym); 
        };
        vector res_sym = broyden_symmetrized(f_sym, n_start, acc_tol);

        std::cout << std::left << std::setw(8) << "" 
                    << std::setw(12) << "Symmetric" 
                    << std::setw(20) << calls_sym 
                    << std::setw(12) << broyden_steps 
                    << std::scientific << std::setprecision(4) << std::setw(18) << RosenbrockBenchmarker::calculateDeviation(res_sym) << "\n";
        
        std::cout << std::string(70, '-') << "\n";
    }

    std::cout << "\n[INFO] Function Calls tracks the total number of times the solver\nevaluates f(x) during the optimization process.\n\n";

    // --------------------------------------------------------------------------------
    // 6.2 - EXTERNAL EXECUTION TIME BENCHMARKING (VIA GNU TIME)
    // --------------------------------------------------------------------------------
    substep("6.2 - EXTERNAL EXECUTION TIME BENCHMARKING (VIA GNU TIME)");

    std::cout << "[INFO] Source code: See Makefile.\n";
    std::cout << "[INFO] Plot script: See Makefile.\n";
    std::cout << "[INFO] Plot: See 'plot_timing.svg'.\n\n";

    // ================================================================================
    
    return 0;

} // main
