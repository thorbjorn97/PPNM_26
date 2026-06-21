# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
		   -Wshadow \
           -Werror \
		   -Wconversion \

OBJ = main.o rootfinder.o ode.o

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<


run: main
	./main | tee out.txt

plot:
	gnuplot plot.gpi

all: main run plot


clean:
	rm -f main *.o *.txt *.dat *.png *.svg


dev: clean all
