# "test.sh" executable shell script: Compiles and tests the "main.cc" program.

# This script tests all input/output scenarious from the instructions
# The expected output (same for all scenarios) is printed for verification
# Usage:
# 1. Ensure "main.cc" is in the same directory as this script.
# 2. In the terminal, make the script executable and run it:
#       chmod +x test.sh
#       ./test.sh        


#!/bin/bash

echo "Compiling..."
echo "runs:     g++ main.cc -o main"
g++ main.cc -o main

echo
echo "=== Task 1 === (directly in command-line)"
echo "runs:     rm -f Out.txt"
echo "runs:     ./main -n 1 -n 2 -n 3 -n 4 -n 5 > Out.txt"
echo "runs:     cat Out.txt"
rm -f Out.txt
./main -n 1 -n 2 -n 3 -n 4 -n 5 > Out.txt
cat Out.txt

echo
echo "=== Task 1 === (cat file to command-line)"
echo "runs:     rm -f Out.txt inputfile"
echo "runs:     ./main -n 1 -n 2 -n 3 -n 4 -n 5 > Out.txt"
echo "runs:     ./main [dollar](cat inputfile) > Out.txt"
echo "runs:     cat Out.txt"
rm -f Out.txt inputfile
echo "-n 1 -n 2 -n 3 -n 4 -n 5" > inputfile
./main $(cat inputfile) > Out.txt
cat Out.txt

echo
echo "=== Task 2 === (piping to command-line with output redirection)"
echo "runs:     rm -f Out.txt"
echo "runs:     echo 1 2 3 4 5 | ./main > Out.txt"
echo "runs:     cat Out.txt"
rm -f Out.txt
echo 1 2 3 4 5 | ./main > Out.txt
cat Out.txt

echo
echo "=== Task 2 === (input and output redirection)"
echo "runs:     rm -f input.txt Out.txt"
echo "runs:     echo 1 2 3 4 5 > input.txt"
echo "runs:     ./main < input.txt > Out.txt"
echo "runs:     cat Out.txt"
rm -f input.txt Out.txt
echo 1 2 3 4 5 > input.txt
./main < input.txt > Out.txt
cat Out.txt

echo
echo "=== Task 2 === (cat piping to command-line with output redirection)"
echo "runs:     rm -f input.txt Out.txt"
echo "runs:     echo 1 2 3 4 5 > input.txt"
echo "runs:     cat input.txt | ./main > Out.txt"
echo "runs:     cat Out.txt"
rm -f input.txt Out.txt
echo 1 2 3 4 5 > input.txt
cat input.txt | ./main > Out.txt
cat Out.txt

echo
echo "=== Task 3 ==="
echo "runs:     rm -f input.txt my_input_file.txt my_output_file.txt"
echo "runs:     echo 1 2 3 4 5 > my_input_file.txt"
echo "runs:     ./main --input my_input_file.txt --output my_output_file.txt"
echo "runs:     cat my_output_file.txt"
rm -f input.txt my_input_file.txt my_output_file.txt
echo "1 2 3 4 5" > my_input_file.txt
./main --input my_input_file.txt --output my_output_file.txt
cat my_output_file.txt

echo
echo "Expected output in all cases:"
echo "1 0.841471 0.540302"
echo "2 0.909297 -0.416147"
echo "3 0.14112 -0.989992"
echo "4 -0.756802 -0.653644"
echo "5 -0.958924 0.283662"

echo
echo "Done."