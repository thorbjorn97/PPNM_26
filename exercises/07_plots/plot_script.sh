# 'plot_script.sh' bash script: Contains bash commands to automate the plotting of results using gnuplot

#!/bin/bash

set -e # stop on errors

# Clean old outputs
rm -f erf.dat gamma.dat lngamma.dat erf.png gamma.png lngamma.png
echo "Old outputs removed."

# Run program
./main

# === Task 1 === plot erf(x) and tabulated values (from Wikipedia)
gnuplot << EOF
set terminal pngcairo size 900,600
set output "erf.png"

set title "Error function erf(x)"
set xlabel "x"
set ylabel "erf(x)"
set grid
set key top left

plot \
    "erf.dat" index 0 with lines title "erf(x)", \
    "erf.dat" index 1 with points pt 7 title "Tabulated values"
EOF

echo
echo "Task 1: Plot saved to erf.png"

# === Task 2 === plot sgamma(x) and tabulated values (from Γ(n) = (n-1)! for positive integers)
gnuplot << EOF
set terminal pngcairo size 900,600
set output "gamma.png"

set title "Approximation of Γ(x)"
set xlabel "x"
set ylabel "Γ(x)"
set grid

set key bottom right
set xtics 1
set xrange [-5:5]
set yrange [-8:8]

plot \
    "gamma.dat" index 0 with lines title "Approximation of Γ(x) (Stirling-based)", \
    "gamma.dat" index 1 with points pt 7 title "Tabulated values (Factorials: Γ(n) = (n-1)!)"
EOF

echo "Task 2: Plot saved to gamma.png"

# === Task 3 === plot lngamma(x) and tabulated values (from std::lgamma(n))
gnuplot << EOF
set terminal pngcairo size 900,600
set output "lngamma.png"

set title "Approximation of ln(Γ(x))"
set xlabel "x"
set ylabel "ln(Γ(x))"
set grid

set key bottom right
set xtics 1
set xrange [0:15]
set yrange [-5:30]

plot \
    "lngamma.dat" index 0 with lines title "Approximation of ln(Γ(x)) (Stirling-based)", \
    "lngamma.dat" index 1 with points pt 7 title "Reference values (using std::lgamma(n))"
EOF

echo "Task 3: Plot saved to lngamma.png"

# end