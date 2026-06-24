# Makefile

CXX       = g++
CXXFLAGS  = -std=c++23 -O2 \
            -Wall -Wextra -Wpedantic \
            -Wshadow \
            -Werror \
            -Wconversion

TARGET    = main

OBJS      = main.o
PLOT_SCR  = plot.gpi
OUTPUTS   = out.txt *.dat *.png *.svg *.tmp *.log log out err

all: $(TARGET) run plot

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

run: $(TARGET)
	@echo "Running matrix timing benchmark..."
	> out.times.dat
	for N in $$(seq 100 20 500); do \
		time --format "$$N %e" --output out.times.dat --append \
		./$(TARGET) -size $$N 1>out 2>err ; \
	done
	@echo "Benchmark data written to out.times.dat"

plot: run
	gnuplot $(PLOT_SCR) 2>&1 | tee -a out.txt

clean:
	rm -f $(TARGET) $(OBJS) $(OUTPUTS)

dev: clean all

.PHONY: all run plot clean dev
