# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
           #-Wconversion -Wshadow \
           #-Werror
TARGET = main

$(TARGET): main.cc evd.cc
	$(CXX) $(CXXFLAGS) main.cc evd.cc -o $(TARGET)

main.o: main.cc evd.h matrix.h vector.h

evd.o: evd.cc evd.h matrix.h vector.h

run: $(TARGET)
	./$(TARGET) | tee out.txt

plot: dr_convergence.dat rmax_convergence.dat plot.gpi
	gnuplot plot.gpi

all: $(TARGET) run plot

clean:
	rm -f *.o main out.txt *.dat *.pdf *.svg *.gp

dev: clean all