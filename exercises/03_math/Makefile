# "Makefile" build file
CXX=g++ # I use g++ as the C++ compiler, so I link with $(CXX)
CXXFLAGS=-std=c++20 -O2 -Wall -Wextra # Flags: C++20 standard, optim. lvl 2, all warnings


# To run the program
all: main run

# The object files are linked to create the executable main
main: main.o sfuns.o
	$(CXX) -o main main.o sfuns.o

# The object files are built from the corresponding source files
sfuns.o: sfuns.h sfuns.cc
	$(CXX) $(CXXFLAGS) -o sfuns.o -c sfuns.cc
main.o: sfuns.h main.cc
	$(CXX) $(CXXFLAGS) -o main.o -c main.cc

run: main # Output of the program is written to Out.txt
	./main > Out.txt

# To clean the directory
clean:  # Remove executable file (main), all object files (*.o), and all output files (Out.txt)
	rm -f main *.o *.txt

dev: clean all

.PHONY: all run plot clean dev

# end
