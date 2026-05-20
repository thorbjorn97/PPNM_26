# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
		   -Wshadow \
           -Werror \
		   #-Wconversion \

TARGET = main

OBJ = main.o lsfit.o

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

main.o: main.cc lsfit.h matrix.h vector.h
	$(CXX) $(CXXFLAGS) -c main.cc

lsfit.o: lsfit.cc lsfit.h matrix.h vector.h
	$(CXX) $(CXXFLAGS) -c lsfit.cc

run: $(TARGET)
	./$(TARGET) | tee out.txt

plot: plot.gpi ThX.dat fit.dat
	gnuplot plot.gpi

all: $(TARGET) run plot

clean:
	rm -f *.o main out.txt *.dat *.png *.svg *.gp *.pdf

dev: clean all