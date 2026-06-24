# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
		   -Wshadow \
           -Werror \
		   -Wconversion \

TARGET    = main

OBJS      = main.o minimizer.o
PLOT_SCR  = plot.gpi
OUTPUTS   = out.txt log *.dat *.png *.svg *.tmp

all: $(TARGET) run plot

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

run: $(TARGET) # Task B: added < higgs.data.txt 2> log as per instructions 
	./$(TARGET) < higgs.data.txt 2> log | tee out.txt

plot: run # Task A+B: added rm -f *.tmp to remove temporary files
	gnuplot $(PLOT_SCR)
	rm -f *.tmp

clean:
	rm -f $(TARGET) $(OBJS) $(OUTPUTS)

dev: clean all

.PHONY: all run plot clean dev