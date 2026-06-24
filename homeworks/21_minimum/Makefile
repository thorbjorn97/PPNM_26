# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
		   -Wshadow \
           -Werror \
		   -Wconversion \

OBJ = main.o minimizer.o

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<


run: main # Task B: added < higgs.data.txt 2> log as per instructions 
	./main < higgs.data.txt 2> log | tee out.txt

plot:
	gnuplot plot.gpi
	rm -f rosen_contours_mesh.tmp himmel_contours_mesh.tmp

all: main run plot

clean: # Task B: changed *.txt -> out.txt to avoid deleting higgs.data.txt
	rm -f main *.o out.txt *.dat *.png *.svg *.tmp 


dev: clean all