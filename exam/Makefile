# Makefile

CXX = g++
CXXFLAGS = -std=c++23 -O2 \
           -Wall -Wextra -Wpedantic \
		   -Wshadow \
           -Werror \
		   -Wconversion

TARGET    = main

OBJS      = main.o broyden.o
PLOT_SCR  = plot.gpi
OUTPUTS   = out.txt *.dat *.svg *.tmp *.log log *.gp

all: $(TARGET) run plot timing_results.dat plot_timing.svg

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

run: $(TARGET)
	./$(TARGET) | tee out.txt

plot:
	gnuplot $(PLOT_SCR)
	rm -f *.tmp

# Execution time profile generation using external timer via GNU Time
# (runs multiple times for low dimensions and takes average since /usr/bin/time cannot measure nanosecond-scale precision)
MAX_DIM = 10  #  Maximum dimension to profile (reduce for faster execution time)
timing_results.dat: main
	@rm -f timing_std.dat timing_sym.dat
	@for d in $$(seq 2 $(MAX_DIM)); do \
		if [ $$d -le 2 ];    then it_std=110; it_sym=110; \
		elif [ $$d -le 4 ];  then it_std=15;  it_sym=100; \
		elif [ $$d -le 8 ];  then it_std=12;  it_sym=100; \
		elif [ $$d -le 16 ]; then it_std=5;   it_sym=80;  \
		elif [ $$d -le 32 ]; then it_std=2;   it_sym=45;  \
		elif [ $$d -le 64 ]; then it_std=1;   it_sym=14;  \
		elif [ $$d -le 90 ]; then it_std=1;   it_sym=6;   \
		else                      it_std=1;   it_sym=3;   \
		fi; \
		pct=$$(( ($$d - 2) * 100 / ($(MAX_DIM) - 2) )); \
		filled=$$(( pct / 4 )); \
		bar=""; i=0; \
		while [ $$i -lt $$filled ]; do bar="$$bar#"; i=$$((i+1)); done; \
		while [ $$i -lt 25 ]; do bar="$$bar-"; i=$$((i+1)); done; \
		printf "\r\033[K[RUNNING]: Profiling... [%s] %3d%% (Dim %3d/$(MAX_DIM))" "$$bar" "$$pct" "$$d"; \
		/usr/bin/time -f "%e" -o tmp_std.txt sh -c "for i in \$$(seq $$it_std); do ./main $$d std > /dev/null; done"; \
		/usr/bin/time -f "%e" -o tmp_sym.txt sh -c "for i in \$$(seq $$it_sym); do ./main $$d sym > /dev/null; done"; \
		awk -v d=$$d -v it=$$it_std '{print d, $$1 / it}' tmp_std.txt >> timing_std.dat; \
		awk -v d=$$d -v it=$$it_sym '{print d, $$1 / it}' tmp_sym.txt >> timing_sym.dat; \
	done
	@echo "\n--> External time benchmarking of multi-dimensional Rosenbrock complete."
	@echo "[INFO] Timing data saved to 'timing_std.dat' and 'timing_sym.dat'."
	@rm -f tmp_std.txt tmp_sym.txt
	@touch timing_results.dat

# Comparative plotting target  # set logscale x 2; \#
plot_timing.svg: timing_results.dat
	@printf "[RUNNING] Plotting timing comparison via Gnuplot...\n"
	@gnuplot -e "\
		set terminal svg background 'white' size 800,500; \
		set output 'plot_timing.svg'; \
		set logscale y; \
		set title 'Quasi-Newton Runtime Scaling Comparison (GNU Time)'; \
		set xlabel 'Dimension (N)'; \
		set ylabel 'Execution Time (seconds)'; \
		set grid; \
		plot 'timing_std.dat' using 1:2 with linespoints lw 2 lc rgb '#d62728' pt 7 title 'Standard Broyden', \
		     'timing_sym.dat' using 1:2 with linespoints lw 2 lc rgb '#1f77b4' pt 5 title 'Symmetrized Broyden';"
	@echo "--> Plotting complete. Saved to 'plot_timing.svg'.\n\n"

clean:
	rm -f $(TARGET) $(OBJS) $(OUTPUTS) timing_std.dat timing_sym.dat plot_timing.svg

dev: clean all

.PHONY: all run plot clean dev
