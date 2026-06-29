# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
		   -Wshadow \
           -Werror \
		   -Wconversion \

TARGET    = main

OBJS      = main.o integrator_mc.o integrator_qmc.o integrator_stratified.o test_functions.o
PLOT_SCR  = plot.gpi
OUTPUTS   = out.txt *.dat *.png *.svg *.tmp *.log log

all: $(TARGET) run plot

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

run: $(TARGET)
	./$(TARGET) | tee out.txt

plot:
	gnuplot $(PLOT_SCR)

clean:
	rm -f $(TARGET) $(OBJS) $(OUTPUTS)

dev: clean all

.PHONY: all run plot clean dev
