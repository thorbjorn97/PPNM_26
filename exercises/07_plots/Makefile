# "Makefile" build file: Defines how to compile and link the program from the source files

CXX = g++ # Use g++ as C++ compiler, so link with $(CXX)
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra # Flags: C++20, optim. lvl 2, all warnings

.PHONY: all run plot clean # declare phony targets

# --- default target ---
all: main run plot

# --- high-level targets ---
run: main # duplicate stdout to both terminal and Out.txt using piping and tee command
	./main | tee Out.txt

plot: main # generate .dat files and .png plots using gnuplot bash script plot_script.sh
	bash plot_script.sh

# --- build targets ---
main: main.o functions.o # link object file into executable main
	$(CXX) $(CXXFLAGS) -o main main.o functions.o

main.o: main.cc functions.h # build object file from corresponding source files
	$(CXX) $(CXXFLAGS) -c main.cc

functions.o: functions.cc functions.h # build object file from corresponding source files
	$(CXX) $(CXXFLAGS) -c functions.cc

# --- clean targets ---
clean: # remove all generated files in current directory
	rm -f *.o main *.dat *.png Out.txt

# end