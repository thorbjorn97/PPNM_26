CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
           -Wconversion -Wshadow \
           -Werror
TARGET = main

GENERATED = $(TARGET) \
            out err \
			out.txt \
            out.times.data \
            qr_timing.png \
            fit.log fit.log.old

$(TARGET): main.cc
	$(CXX) $(CXXFLAGS) main.cc -o $(TARGET)

run: $(TARGET)
	./$(TARGET) | tee out.txt

clean:
	rm -f $(GENERATED)

out.times.data: $(TARGET)
	>$@
	for N in $$(seq 100 20 500); do \
		time --format "$$N %e" --output $@ --append \
		./$(TARGET) -size $$N 1>out 2>err ;\
	done

qr_timing.png: out.times.data plot.gpi
	gnuplot plot.gpi 2>&1 | tee -a out.txt

dev: clean run out.times.data qr_timing.png