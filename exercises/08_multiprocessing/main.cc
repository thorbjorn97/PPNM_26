// "main.cc" source file: Hosts the main() function

#include <functional>  // Provides std::ref for passing references to threads
#include <iostream>    // Provides std::cout for printing terminal output
#include <string>      // Provides std::string and std::stoi for commnad-line argument handling
#include <thread>      // Provides std::thread for managing multithreading
#include <vector>      // Provides std::vector for storing thread data and thread objects

#include "harm.h"  // Includes the content of the header file "harm.h"

// ================================================================================================
// Task 2: Create a main function that reads nthreads and nterms from command-line
// ================================================================================================
int main(int argc, char **argv) {
    // argc = number of command-line arguments
    // argv = array of C-style strings containing the command-line arguments

    int nthreads = 1;       // default number of threads
    int nterms = (int)1e8;  // default number of harmonic series terms

    for (int i = 0; i < argc; i++) {  // loop over command-line arguments
        std::string arg = argv[i];    // convert argv[i] to std::string for easier handling

        // If argv[i] is either "-nthreads" or "-nterms" and argv[i + 1] exists,
        // parse argv[i + 1] as an integer and store it in nthreads or nterms, respectively:
        if (arg == "-nthreads" && i + 1 < argc) nthreads = std::stoi(argv[i + 1]);
        if (arg == "-nterms" && i + 1 < argc) nterms = std::stoi(argv[i + 1]);
    }

    // ============================================================================================
    // Task 3: Prepare data-objects to be used locally in separate threads
    // ============================================================================================
    // Create a std::vector object named params containing nthreads elements of type data; by
    // definition of data in "harm.h", each element params[i] has fields a, b, sum:
    std::vector<data> params(nthreads);

    for (int i = 0; i < nthreads; i++) {  // loop over thread indices and assign index ranges [a,b)

        // Integer division nterms / nthreads gives the base number of terms assigned to each
        // thread; multiplying by i and (i + 1) gives the start and end indices:
        params[i].a = 1 + nterms / nthreads * i;        // assign start index a
        params[i].b = 1 + nterms / nthreads * (i + 1);  // assign end index b (exclusive)
        params[i].sum = 0;                              // initialize partial sum for thread i
    }

    // Ensure the last thread gets all remaining terms (corrects for integer division rounding):
    params[params.size() - 1].b = nterms + 1;

    // ============================================================================================
    // Task 4: Prepare the threads and run them in parallel
    // ============================================================================================

    // Create an empty std::vector object named threads for storing elements of type std::thread:
    std::vector<std::thread> threads;

    // Reserve capacity in threads for nthreads elements of type std::thread (no construction yet):
    threads.reserve(nthreads);

    for (int i = 0; i < nthreads; i++) {  // iterate over thread indices (i = 0,...,nthreads - 1)

        // Construct via emplace_back() a std::thread that immediately executes harm(data& arg),
        // with arg bound by std::ref(params[i]) to params[i], such that arg aliases params[i]
        // and the thread writes its result directly into params[i].sum:
        threads.emplace_back(harm, std::ref(params[i]));
    }

    // ============================================================================================
    // Task 5: Join the threads
    // ============================================================================================

    // Join all std::thread objects in threads, blocking the main thread until all
    // worker threads have finished execution of harm(data& arg):
    for (auto &thread : threads) thread.join();

    // ============================================================================================
    // Task 6: Calculate the total sum
    // ============================================================================================

    // Reduce partial results from all threads by summing all params[i].sum values into total:
    double total = 0;       // initialize total to zero before summing
    for (data &p : params)  // loop over all data objects p in params
        total += p.sum;     // add p.sum to total for each p (reference used to avoid copying)

    // ============================================================================================

    // Print the result:
    std::cout << "nthreads: " << nthreads << "\n";
    std::cout << "nterms: " << nterms << "\n";
    std::cout << "Harmonic sum: " << total << "\n";

    return 0;
}

// end