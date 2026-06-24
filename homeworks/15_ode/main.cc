// main.cc

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "ode.h"
#include "matrix.h"
#include "vector.h"
#include "utils.h"
#include "models.h"

using namespace pp;

int main(){
    // ============================================================================================
    // Task A (6 points): Embedded rule Runge-Kutta ODE integrator
    // ============================================================================================
    section("Task A (6 points): Embedded rule Runge-Kutta ODE integrator");

    // ====================================================
    // A.1 - RUNGE-KUTTA STEPPER
    // ====================================================
    substep("A.1 - RUNGE-KUTTA STEPPER");

    std::cout << "See ode.cc (implementation of rkstep12)\n";

    // ====================================================
    // A.2 - ADAPTIVE STEP-SIZE DRIVER
    // ====================================================
    substep("A.2 - ADAPTIVE STEP-SIZE DRIVER");

    std::cout << "See ode.cc (implementation of driver)\n";

    // ====================================================
    // A.3 - DEBUGGING ROUTINES
    // ====================================================
    substep("A.3 - DEBUGGING ROUTINES");

    std::cout << "IMPLEMENTATION: See models.cc (implementation of harmonic)\n\n";

    std::cout << "DATAFILE CREATION:\n";
    
    // t1[i] = time at integration point i
    // y1[i] = state vector at time t1[i]

    auto [t1, y1] = driver(
        harmonic,               // function
        0,                      // initial time
        30,                     // final time
        vector{1.0, 0.0},       // initial state: [u(0), u'(0)]
        0.1,                    // initial step size
        1e-8,                   // accuracy
        1e-8                    // precision
    );

    // time series
    std::ofstream out1("out_harmonic.dat");
    for (size_t i = 0; i < t1.size(); i++) {
        out1 << t1[i] << " " << y1[i][0] << " " << y1[i][1] << "\n";
    }
    out1.close();
    std::cout << "Datafile created: out_harmonic.dat\n\n";

    std::cout << "DEBUGGING RESULT:\n";

    // // phase space
    // std::ofstream out1_phase("out_harmonic_phase.dat");
    // for(size_t i = 0; i < x1.size(); i++){
    //     out1_phase << y1[i][0] << " " << y1[i][1] << "\n";
    // }
    // out1_phase.close();

    // error check
    double max_err = 0.0;
    for (size_t i = 0; i < t1.size(); i++) {
        double err = std::abs(y1[i][0] - std::cos(t1[i]));
        max_err = std::max(max_err, err);
    }

    std::cout << "Harmonic max error: " << max_err << "\n";
    std::cout << "Final step size (harmonic): adaptive (not stored)\n";

    // Energy drift check
    double E0 = 0.5 * (y1.front()[0]*y1.front()[0] + y1.front()[1]*y1.front()[1]);
    double E1 = 0.5 * (y1.back()[0]*y1.back()[0] + y1.back()[1]*y1.back()[1]);

    std::cout << "Harmonic energy drift: " << std::abs(E1 - E0) << "\n";

    // ====================================================
    // A.4 - REPRODUCTION OF EXAMPLES
    // ====================================================
    substep("A.4 - REPRODUCTION OF EXAMPLES");

    std::cout << "IMPLEMENTATION: See models.cc (implementation of friction and lotka)\n\n";

    std::cout << "DATAFILE CREATION:\n";
    // oscillator with friction

    auto [t2, z2] = driver(
        friction,               // function
        0,                      // initial time t
        10,                     // final time t
        vector{M_PI - 0.1, 0},  // initial state: [theta(0) = π, omega(0) = 0]
        0.1,                    // initial step size
        1e-8,                   // accuracy
        1e-8                    // precision
    );

    std::ofstream out2("out_friction.dat");
    for (size_t i = 0; i < t2.size(); i++) {
        out2 << t2[i] << " " << z2[i][0] << " " << z2[i][1] << "\n";
    }
    out2.close();
    std::cout << "Datafile created: out_friction.dat\n";

    // Lotka-Volterra system
    auto [t3, z3] = driver(
        lotka,                  // function
        0,                      // initial time t
        15,                     // final time t
        vector{10, 5},          // initial state: [x(0) = 10, x'(0) = 0]
        0.1,                    // initial step size
        1e-8,                   // accuracy
        1e-8                    // precision
    );

    std::ofstream out3("out_lotka.dat");
    for (size_t i = 0; i < t3.size(); i++) {
        out3 << t3[i] << " " << z3[i][0] << " " << z3[i][1] << "\n";
    }
    out3.close();
    std::cout << "Datafile created: out_lotka.dat\n\n";

    std::cout << "REPRODUCTION RESULT:\n";

    std::cout << "Friction final angle: " << z2.back()[0] << "\n";
    std::cout << "Friction final angular velocity: " << z2.back()[1] << "\n";

    std::cout << "Lotka final (prey, predator): "
              << z3.back()[0] << ", " << z3.back()[1] << "\n";

    // ============================================================================================
    // Task B (3 points): Relativistic precession of planetary orbit
    // ============================================================================================
    section("Task B (3 points): Relativistic precession of planetary orbit");

    // ====================================================
    // DEFINITION OF PLANETARY ORBIT ODEs
    // ====================================================
    substep("DEFINITION OF PLANETARY ORBIT ODEs");
    auto orbit_cir = [](double phi, vector y) { return orbit(phi, y, 0.0); };
    auto orbit_ell = [](double phi, vector y) { return orbit(phi, y, 0.0); };
    auto orbit_rel = [](double phi, vector y) { return orbit(phi, y, 0.01); };

    std::cout << "IMPLEMENTATION: See models.cc\n\n";

    // ====================================================
    // SOLUTION OF PLANETARY ORBIT ODEs
    // ====================================================
    substep("SOLUTION OF PLANETARY ORBIT ODEs");
    // B.1 Circular Newtonian orbit (ε = 0, u(0) = 1, u'(0) = 0)
    auto [tB1, uB1] = driver(
        orbit_cir,              // function (ODE)
        0,                      // initial angle φ
        50,                     // final angle φ
        vector{1.0, 0.0},       // initial state: [u(0), u'(0)]
        0.1,                    // initial step size
        1e-8,                   // accuracy
        1e-8                    // precision
    );
    std::ofstream outB1("out_orbit_circular.dat");
    for (size_t i = 0; i < tB1.size(); i++) {
        outB1 << tB1[i] << " " << uB1[i][0] << "\n";
    }
    outB1.close();
    std::cout << "B1 circular orbit complete\n";

    // B.2 Newtonian elliptical (ε = 0, u(0) = 1, u'(0) = -0.5)
    auto [tB2, uB2] = driver(
        orbit_ell,              // function (ODE)
        0,                      // initial angle φ
        50,                     // final angle φ
        vector{1.0, -0.5},      // initial state: [u(0), u'(0)]
        0.1,                    // initial step size
        1e-8,                   // accuracy
        1e-8                    // precision
    );
    std::ofstream outB2("out_orbit_elliptical.dat");
    for (size_t i = 0; i < tB2.size(); i++) {
        outB2 << tB2[i] << " " << uB2[i][0] << "\n";
    }
    outB2.close();
    std::cout << "B2 Newtonian ellipse complete\n";

    // B.3 Relativistic precession (ε = 0.01, u(0) = 1, u'(0) = -0.5)
    auto [tB3, uB3] = driver(
        orbit_rel,              // function (ODE)
        0,                      // initial angle φ
        80,                     // final angle φ
        vector{1.0, -0.5},      // initial state: [u(0), u'(0)]
        0.1,                    // initial step size
        1e-8,                   // accuracy
        1e-8                    // precision
    );
    std::ofstream outB3("out_orbit_relativistic.dat");
    for (size_t i = 0; i < tB3.size(); i++) {
        double r = 1.0 / uB3[i][0];              // radius
        double x = r * std::cos(tB3[i]);         // x
        double y = r * std::sin(tB3[i]);         // y

        outB3 << x << " " << y << "\n";
    }
    outB3.close();
    std::cout << "B3 relativistic orbit complete\n";

    // ============================================================================================
    // Task C (1 points)
    // ============================================================================================
    section("Task C (1 points)");
    
    // C.1 - Reproduce figure 8 stable three body solution using numerical ODE integrator

    vector z0{
    0.4662036850,
    0.4323657300,

    0.4662036850,
    0.4323657300,

    -0.93240737,
    -0.86473146,

    -0.97000436,
    0.24308753,

    0.97000436,
    -0.24308753,

    0.0,
    0.0
    };

    auto [t,z]=driver(
        threebody,
        0,
        2.105,
        z0,
        0.01,
        1e-8,
        1e-8
    );

    // Write trajectory data file
    std::ofstream outC("out_threebody.dat");
    for(size_t i=0;i<t.size();i++){

        outC
        << t[i] <<" "
        << z[i][6] <<" "
        << z[i][7] <<" "

        << z[i][8] <<" "
        << z[i][9] <<" "

        << z[i][10]<<" "
        << z[i][11]

        << "\n";
    }
    outC.close();

    std::cout << "C complete\n\n\n";

    return 0;

} // main