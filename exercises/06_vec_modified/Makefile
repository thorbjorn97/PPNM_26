# "Makefile" build file
CXX = g++ # Use g++ as C++ compiler, so link with $(CXX)
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra # Flags: C++20, optim. lvl 2, all warnings

# Link object file into executable main
main: main.o
	$(CXX) $(CXXFLAGS) -o main main.o

# Build object file from corresponding source files
main.o: main.cc vec.h
	$(CXX) $(CXXFLAGS) -c main.cc

# Output of the program is written to Out.txt
Out.txt: main
	./main > Out.txt

# Remove all generated files to clean directory
clean:
	rm -f *.o main Out.txt
	
# end