// "main.cc" source file: Hosts main() function
#include <iostream>     // Provides std::cin, std::cout, std::cerr
#include <vector>       // Provides std::vector
#include <string>       // Provides std::string
#include <cmath>        // Provides std::sin and std::cos
#include <cstdlib>      // Provides EXIT_SUCCESS and EXIT_FAILURE
#include <fstream>      // Provides std::ifstream and std::ofstream


int main(int argc, char* argv[]) {
// main() function with command-line arguments:
// argc: number of command-line arguments
// argv: array of strings representing the arguments

    // ----- Parse all arguments first (restructuring of scripts in instructions)
    std::vector<double> numbers;
    // Task 1: container to store command-line numbers
    
    std::string infile="", outfile="";
    // Task 3: strings to store filenames for input and output

    for(int i=0;i<argc;++i){
    // Loop over all command-line arguments (argv[0] to argv[argc-1])

        std::string arg=argv[i];
        // Convert C-style string argv[i] to std::string for easier comparison        
        
        if(arg=="-n" && i+1<argc) numbers.push_back(std::stod(argv[i+1]));
        // Task 1: check if current arg is "-n" and ensure a next argument exists
        // If so, convert next argument to double and store in numbers vector

        if(arg=="--input" && i+1<argc) infile=argv[i+1];
        // Task 3: check if current arg is "--input" and ensure a next argument exists
        // If so, store the next argument as the input filename

        if(arg=="--output" && i+1<argc) outfile=argv[i+1];
        // Task 3: check if current arg is "--output" and ensure a next argument exists
        // If so, store the next argument as the output filename
    }

    // ----- Task 1: Command-line input
    if(!numbers.empty()){ // {new}
    // Condition for selecting Task 1 as the active mode
    // Checks if ANY "-n" args were provided (as these are stored in "numbers")
    // !numbers.empty() == true   --> at least one number exists --> run
    // !numbers.empty() == false  --> no command-line "numbers"  --> skip

        for(auto n: numbers)
        // Iterate over each number "n" collected in the vector "numbers"

            std::cout << n <<" "<< std::sin(n) <<" "<< std::cos(n) << std::endl;
            // Output: n, sin(n), cos(n), separated by spaces
        
        exit(EXIT_SUCCESS);
        // Terminate program to ensure only one task is executed (prevents fall-through into execution of mode for Task 2 or 3)
    }

    // ----- Task 2: Standard input
    if(infile.empty() && outfile.empty()){ // {new}
    // Condition for selecting Task 2 as the active mode
    // Checks if NO file args (neither "--input" NOR "--output") were provided
    // infile.empty() && outfile.empty() == true   --> "--input" NOR "--output"  --> run
    // infile.empty() && outfile.empty() == false  --> "--input" OR "--output"   --> skip

        double x;
        // Variable to hold each standard input value "x" read from std::cin

        while(std::cin >> x){
            // Read values one at a time from standard input (loops until EOF or invalid input)

            std::cout << x <<" "<< std::sin(x) <<" "<< std::cos(x) << std::endl;
            // Print to standard output: x, sin(x), cos(x), separated by spaces
        }

        exit(EXIT_SUCCESS);// {new}
        // Terminate program to ensure only one task is executed (prevents fall-through into execution of mode for Task 3)
    }

    // ----- Task 3: File streams
    if(!infile.empty() && !outfile.empty()){ // {new}
    // Condition for selecting Task 3 as the active mode
    // Checks if BOTH file args ("--input" AND "--output") were provided
    // !infile.empty() && !outfile.empty() == true   --> "--input" AND "--output"   --> run
    // !infile.empty() && !outfile.empty() == false  --> "--input" NAND "--output"  --> skip

        std::ifstream myinput(infile);
        std::ofstream myoutput(outfile);
        // Open input file for reading and output file for writing
        
        double x;
        // Variable to hold each value "x" read from input file

        if(myinput.is_open() && myoutput.is_open()){
        // Check that both files were successfully opened. If so, run
            while(myinput >> x){
            // Read values one at a time from input file

                myoutput << x <<" "<< std::sin(x) <<" "<< std::cos(x) << std::endl;
                // Write to output file: x, sin(x), cos(x), separated by spaces    
            }    
        }
        else{
            std::cerr << "Error opening files: " << infile << " " << outfile << std::endl;
            // If either file fails to open, print error message to standard error stream
            
            return EXIT_FAILURE;
            // Terminate program with failure status
        }

        myinput.close();
        myoutput.close();
        // Explicitly close input and output files

        exit(EXIT_SUCCESS);
        // Terminate program after successful file processing
    }

    return 0; // {new}
    // Fallback return if no task was executed

}

// end
