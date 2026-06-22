// main.cc

#include <iostream>
#include <cmath>
#include <fstream>

#include "linterp.h"
#include "fspline.h"
#include "qspline.h"
#include "cspline.h"
#include "utils.h"

using namespace pp;

int main(){
    // ============================================================================================
    // Task A (6 points): Linear spline (linear interpolation)
    // ============================================================================================
    section("Task A (6 points): Linear spline (linear interpolation)");

    // ==========================================
    // INPUT DATA
    // ==========================================
    substep("INPUT DATA (point set reused in all tasks)");

    // Create point set for testing: {xi = 0,1,...,9; yi = cos(xi)}
    vector x, y;
    for(double xi=0; xi<=9; xi+=1.0){
        x.push_back(xi);
        y.push_back(cos(xi));
    }

    // Print points and write them to file nodes.dat
    std::cout << "Point set: {xi = 0,1,...,9; yi = cos(xi)}:\n";
    std::ofstream nodes("nodes.dat");
    for(size_t i=0;i<x.size();i++){
        std::cout << x[i] << " " << y[i] << "\n";
        nodes << x[i] << " " << y[i] << "\n";
    }
    nodes.close();
    std::cout << "\nDatafile created: nodes.dat\n";

    // Corresponding dense function values (for comparison in plots)
    std::ofstream f("exact.dat");
    for(double z=0; z<=9; z+=0.05){
        f << z << " "
           << cos(z) << " "     // Exact function value at z
           << sin(z) << " "     // integral of exact function from x[0] to z
           << -sin(z) << "\n";  // derivative of exact function at z 
    }
    f.close();
    std::cout << "Datafile created: exact.dat\n";
    
    // ==========================================
    // IMPLEMENTATION
    // ==========================================
    substep("IMPLEMENTATION");

    std::cout << "See linterp.h and linterp.cc.\n";

    // ==========================================
    // OUTPUT DATA
    // ==========================================
    substep("OUTPUT DATA");

    // Compute and write spline values and integrals to file out_lin.dat
    std::ofstream fa("out_lin.dat");
    for(double z=0; z<=9; z+=0.05){
        fa << z << " "
           << linterp(x, y, z) << " "               // linear spline value at z
           << linterpInteg(x, y, z) << " "          // integral of linear spline from x[0] to z
           << linterp(x, y, z) - cos(z) << "\n";    // deviation from exact function
    }
    fa.close();
    std::cout << "Datafile created: out_lin.dat\n";

    // ==========================================
    // INDICATIVE PLOT
    // ==========================================
    substep("INDICATIVE PLOT");

    std::cout << "See plot.gpi and splines_results.svg (spline plots)\n";
    std::cout << "See plot_dif.gpi and splines_deviations.svg (spline deviation plots)\n";

    // ============================================================================================
    // Task B (3 points): Quadratic spline
    // ============================================================================================
    section("Task B (3 points): Quadratic spline");

    // ==========================================
    // IMPLEMENTATION
    // ==========================================
    substep("IMPLEMENTATION");

    std::cout << "See qspline.h and qspline.cc.\n";

    // ==========================================
    // OUTPUT DATA
    // ==========================================
    substep("OUTPUT DATA");

    // Construct object spline of type qspline from point set (x, y)
    qspline quad(x, y); // coefficients (b, c) are precomputed here

    // Compute and write spline values, derivatives and integrals to file out_quad.dat
    std::ofstream fb("out_quad.dat");
    for(double z=0; z<=9; z+=0.05){ // loop densely over [0,9]
        fb << z << " "
           << quad.eval(z) << " "             // evaluate quadratic spline at z
           << quad.integ(z) << " "            // evaluate integral of quadratic spline from x[0] to z
           << quad.deriv(z) << " "            // evaluate derivative of quadratic spline at z
           << quad.eval(z) - cos(z) << "\n";  // deviation from exact function
        }
    fb.close();
    std::cout << "Datafile created: out_quad.dat\n";

    // ==========================================
    // INDICATIVE PLOT
    // ==========================================
    substep("INDICATIVE PLOT");

    std::cout << "See plot.gpi and splines_results.svg (spline plots)\n";
    std::cout << "See plot_dif.gpi and splines_deviations.svg (spline deviation plots)\n";

    // ==========================================
    // DEBUGGING
    // ==========================================
    substep("DEBUGGING");

    vector x_test={1,2,3,4,5};

    vector y1={1,1,1,1,1};
    vector y2={1,2,3,4,5};
    vector y3={1,4,9,16,25};

    qspline qs1(x_test,y1);
    qspline qs2(x_test,y2);
    qspline qs3(x_test,y3);

    std::cout << "\nCASE: y = 1\n";
    std::cout << "Expected:\n";
    std::cout << "b = { 0 0 0 0 }\n";
    std::cout << "c = { 0 0 0 0 }\n";

    std::cout << "Program: \n";
    std::cout << "b = { ";
    for(size_t i=0;i<qs1.b.size();i++)
        std::cout<<qs1.b[i]<<" ";
    std::cout << "}\n";
    std::cout << "c = { ";
    for(size_t i=0;i<qs1.c.size();i++)
        std::cout<<qs1.c[i]<<" ";
    std::cout << "}\n";
    std::cout<<"Interpolation test: qs(2.5) = " <<qs1.eval(2.5) <<" (expected 1)\n";

    std::cout << "\nCASE: y = x\n";

    std::cout << "Expected:\n";
    std::cout << "b = { 1 1 1 1 }\n";
    std::cout << "c = { 0 0 0 0 }\n";

    std::cout << "Program: \n";
    std::cout << "b = { ";
    for(size_t i=0;i<qs2.b.size();i++)
        std::cout<<qs2.b[i]<<" ";
    std::cout << "}\n";
    std::cout << "c = { ";
    for(size_t i=0;i<qs2.c.size();i++)
        std::cout<<qs2.c[i]<<" ";
    std::cout << "}\n";
    std::cout<<"Interpolation test: qs(2.5) = " <<qs2.eval(2.5) <<" (expected 2.5)\n";

    std::cout << "\nCASE: y = x²\n";
    std::cout << "Expected (average of forward and backward recursion):\n";
    std::cout << "b = { 2 4 6 8 }\n";
    std::cout << "c = { 1 1 1 1 }\n";

    std::cout << "Program: \n";
    std::cout << "b = { ";
    for(size_t i=0;i<qs3.b.size();i++)
        std::cout<<qs3.b[i]<<" ";
    std::cout << "}\n";
    std::cout << "c = { ";
    for(size_t i=0;i<qs3.c.size();i++)
        std::cout<<qs3.c[i]<<" ";
    std::cout << "}\n";
    std::cout<<"Interpolation test: qs(2.5) = " <<qs3.eval(2.5) <<" (expected 6.25)\n";

    // ============================================================================================
    // Task C (1 point): Functional Programming and/or Cubic Spline
    // ============================================================================================
    section("Task C (1 point): Functional Programming and/or Cubic Spline");

    // ==========================================
    // IMPLEMENTATION
    // ==========================================
    substep("IMPLEMENTATION");

    std::cout << "See fspline.h and fspline.cc (quadratic Spline, functional)\n";
    std::cout << "See cspline.h and cspline.cc (cubic Spline, OOP)\n";

    // ==========================================
    // OUTPUT DATA
    // ==========================================
    substep("OUTPUT DATA");

    // QUADRATIC SPLINE (functional)
    auto fq = make_qspline(x,y);
    std::ofstream fc("out_func.dat");
    for(double z=0; z<=9; z+=0.05){
        fc << z    << " "
           << fq(z) << " "
           << fq(z) - cos(z) << "\n";
        }
    fc.close();
    std::cout << "Datafile created: out_func.dat (quadratic spline, functional)\n";

    // CUBIC SPLINE (OOP)
    cspline cubic(x,y);
    std::ofstream fd("out_cubic.dat");
    for(double z=0; z<=9; z+=0.05){
        fd << z << " "
           << cubic.eval(z) << " "            // evaluate cubic spline at z
           << cubic.integ(z) << " "           // evaluate integral of cubic spline from x[0] to z
           << cubic.deriv(z) << " "           // evaluate derivative of cubic spline at z
           << cubic.eval(z) - cos(z) << "\n"; // deviation from exact function

        }
    fd.close();
    std::cout << "Datafile created: out_cubic.dat (cubic spline, OOP)\n";

    // ==========================================
    // INDICATIVE PLOTS
    // ==========================================
    substep("INDICATIVE PLOTS");

    std::cout << "See plot.gpi and splines_results.svg (spline plots)\n";
    std::cout << "See plot_dif.gpi and splines_deviations.svg (spline deviation plots)\n\n";

    return 0;

} // main
