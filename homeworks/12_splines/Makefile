# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
           -Wshadow \
           -Werror \
		   #-Wconversion \

TARGET = main

OBJ = main.o linterp.o qspline.o fspline.o cspline.o

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

main.o: main.cc linterp.h qspline.h fspline.h vector.h
	$(CXX) $(CXXFLAGS) -c main.cc

linterp.o: linterp.cc linterp.h vector.h
	$(CXX) $(CXXFLAGS) -c linterp.cc

qspline.o: qspline.cc qspline.h vector.h
	$(CXX) $(CXXFLAGS) -c qspline.cc

fspline.o: fspline.cc fspline.h vector.h
	$(CXX) $(CXXFLAGS) -c fspline.cc

cspline.o: cspline.cc cspline.h
	$(CXX) $(CXXFLAGS) -c cspline.cc

run: $(TARGET)
	./$(TARGET) | tee out.txt
	gnuplot plot.gpi plot_dif.gpi

plot:
	gnuplot plot.gpi plot_dif.gpi

clean:
	rm -f *.o $(TARGET) out.txt *.dat *.svg nodes.dat

dev: clean $(TARGET) run
