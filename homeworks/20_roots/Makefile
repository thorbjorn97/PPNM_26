# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
		   -Wshadow \
           -Werror \
		   -Wconversion \

TARGET    = main

OBJS      = main.o rootfinder.o ode.o
PLOT_SCR  = plot.gpi
OUTPUTS   = out.txt log *.dat *.png *.svg *.tmp

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