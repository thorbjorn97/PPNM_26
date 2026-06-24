# "README.txt" file: For instructions on usage

# Instructions (for Makefile usage in terminal):
`make out`   	    <-- to execute the program and save output to Out.txt.
`make out.times`    <-- to execute the program with different numbers of threads and store timing data in out.times.
`make times.svg`    <-- to execute the script "plot.gpi" to generate the timing plot using gnuplot.
`make` 			    <-- to build the program (creates main executable and object files) + do all the above steps
`make clean` 	    <-- to remove all generated files

`make dev`          <-- to perform a complete fresh workflow: equivalent to running
                            `make clean`,
                            `make out`,
                            `out.times`,
                            `make times.svg`
                        in sequence. Or, equivalently,
                            `make clean`,
                            `make`
                        in sequence.

# Manual execution example:
./main -nthreads 4 -nterms 100000000

# end