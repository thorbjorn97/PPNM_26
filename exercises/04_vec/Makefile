# "Makefile" build file
CXX = g++ # Use g++ as C++ compiler, so link with $(CXX)
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra # Flags: C++20, optim. lvl 2, all warnings

# Link object files into executable main
main: main.o vec.o
	$(CXX) $(CXXFLAGS) -o main main.o vec.o

# Build object files from corresponding source files
main.o: main.cc vec.h
	$(CXX) $(CXXFLAGS) -c main.cc
vec.o: vec.cc vec.h
	$(CXX) $(CXXFLAGS) -c vec.cc

# Output of the program is written to Out.txt
Out.txt: main
	./main > Out.txt

# Remove all generated files to clean directory
clean:
	rm -f *.o main Out.txt
	
# end