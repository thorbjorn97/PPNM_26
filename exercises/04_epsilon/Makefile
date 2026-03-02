# "Makefile" build file
CXX=g++ # I use g++ as the C++ compiler, so I link with $(CXX)
CXXFLAGS=-std=c++20 -O2 -Wall -Wextra # Flags: C++20 standard, optim. lvl 2, all warnings

machine_epsilon: main.cc
	$(CXX) $(CXXFLAGS) main.cc -o machine_epsilon

Out.txt: main # Output of the program is written to Out.txt
	./main > Out.txt

# To clean the directory
clean:  # Removes object files (*.o), executable (main) and output file (Out.txt)
	rm -f *.o main Out.txt

# end
