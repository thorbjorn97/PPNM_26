# "Makefile" build file: Defines how to compile and link the program from the source files

# === Compiler configuration ===

CXX = g++  # use g++ as C++ compiler, so link with $(CXX)
CXXFLAGS = -std=c++23 -O2 -Wall -Wextra -pthread  # flags: C++23, optim. lvl 2, all warnings

# === Default target ===

all: main # build main executable by default when 'make' is run without arguments

# === Build targets ===

main: main.o harm.o  # link object file into executable main
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cc harm.h  # build object file from corresponding source files
	$(CXX) $(CXXFLAGS) -c $< -o $@

harm.o: harm.cc harm.h  # build object file from corresponding source files
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === High-level targets ===

out: main  # duplicate stdout to both terminal and Out.txt using piping and tee command
	./main | tee Out.txt

# =================================================================================================
# Task 7: Use POSIX time command to measure runtime of main with different number of threads
# =================================================================================================

# Define number of harmonic terms to compute in each iteration over nthreads
N = 1000000000

# Define POSIX time command used to measure execution time of main and append output to out.times
# (Output format: nthreads elapsed_time user_cpu_time)
TIME = /usr/bin/time --format "$$nthreads %e %U" --append --output out.times

# Loop over different thread counts, benchmark each configuration and log results to log.main
out.times: main
	>$@
	>log.main
	for nthreads in 1 2 3 4 5 6 7 8; do \
		$(TIME) ./main -nthreads $$nthreads -nterms $(N) >> log.main;\
	done

# =================================================================================================
# Task 8: Make a plot 'running time' versus 'number of threads'.
# =================================================================================================

times.svg: out.times plot.gpi  # run gnuplot with plot.gpi to create times.svg from out.times
	gnuplot plot.gpi

# =================================================================================================

# === Convenience targets ===

dev: clean out times.svg  # perform complete fresh workflow (clean, build, output, benchmark, plot)

# === Clean targets ===

clean: # remove all generated files in current directory
	rm -f *.o main Out.txt out.times \
		log.main log.times fit.log *.svg

# === Phony targets ===

.PHONY: all out clean dev  # declare phony targets

# end