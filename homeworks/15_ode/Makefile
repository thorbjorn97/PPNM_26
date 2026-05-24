# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
		   -Wshadow \
           -Werror \
		   #-Wconversion \

TARGET = main

OBJ = main.o ode.o models.o

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


run: $(TARGET)
	./$(TARGET) | tee out.txt

plot:
	gnuplot plot.gpi

all: $(TARGET) run plot


clean:
	rm -f *.o $(TARGET) out.txt *.dat *.png *.svg *.gp *.pdf

dev: clean all