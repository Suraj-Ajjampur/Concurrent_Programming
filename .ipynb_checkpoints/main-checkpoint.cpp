/*****************************************************************
 * @author Suraj Ajjampur
 * @file main.cpp
 * 
 * @brief Multithreaded Program entry point for file sorting using OpenMP
 * 
 * @date 15 Nov 2023
 * 
 * @ref https://www.geeksforgeeks.org/multithreading-in-cpp/
********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include "mergesort.h" 
#include "quicksort.h" 
#include <ctime>
#include <barrier> 
#include <iostream>
#include <getopt.h>
#include <omp.h>

// Define timespec structs for measuring time
struct timespec startTime, endTime;

// Function to print my name
void printName() {
    cout << "Suraj Ajjampur" << endl;
}

// Function to sort integers in a file and print to another file
void sortAndPrintFile(const string& inputFile, const string& outputFile, const string& algorithm) {
    // Open the input file
    ifstream inFile(inputFile);
    
    // Check if the input file opened successfully
    if (!inFile.is_open()) {
        cerr << "Error: Could not open the input file." << endl;
        return;
    }

    // Read integers from the input file into a vector
    vector<int> numbers;
    int num;
    while (inFile >> num) {
        numbers.push_back(num);
    }

    // Close the input file
    inFile.close();

    // Sort the vector based on the selected algorithm
    if (algorithm == "quick") {
       quicksort(numbers, 0, numbers.size() - 1); // Call the quicksort function
    } else if (algorithm == "merge") {
        mergesort(numbers, 0, numbers.size() - 1); // Call the mergesort function
    } else {
        cerr << "Error: Invalid algorithm specified." << endl;
        return;
    }

    // Open the output file
    ofstream outFile(outputFile);

    // Check if the output file opened successfully
    if (!outFile.is_open()) {
        cerr << "Error: Could not open the output file." << endl;
        return;
    }

    // Write sorted integers to the output file
    for (int num : numbers) {
        outFile << num << endl;
    }

    // Close the output file
    outFile.close();
}

/**
 * Main function to process command-line arguments and execute sorting algorithms on input data.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return An integer indicating the exit status.
 */
int main(int argc, char* argv[]) {
    // Check if any command-line arguments are provided
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [--name] [-i sourcefile.txt] [-o outfile.txt] [-t NUMTHREADS] [--alg=<forkjoin,lkbucket>]" << endl;
        return 1;
    }

    // Process command-line arguments
    string inputFile = "";
    string outputFile = "";
    string algorithm = "";
    int NUM_THREADS;
    
    // variable to parse command line arguements
    int c;

    //Define getopt long struct with commands
    static struct option long_options[] = {
        {"name", no_argument, 0, 'm'},
        {"threads", required_argument, 0, 't'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"alg", required_argument, 0, 'a'},
        {0, 0, 0, 0}
    };
    
    while (1) {
        int option_index = 0;

        // Parse the command line options
        c = getopt_long(argc, argv, "mt:a:i:o:", long_options, &option_index);
        
        // Detect the end of the options
        if (c == -1)
            break;
        switch (c) {
            case 'm':
                // Print name
                cout << "Suraj Ajjampur" << endl;
                return 0;

            case 't':
                // Set the number of threads
                NUM_THREADS = stoi(optarg);
                break;

            case 'a':
                // Set the lock type
                algorithm = optarg;
                break;

            case 'i':
                // Set the input file
                inputFile = optarg;

            case 'o':
                // Set the output file
                outputFile = optarg;
                break;

            case '?':
                // Handle invalid options
                cerr << "Error: Invalid option." << endl;
                return 1;

            default:
                abort();
        }
    }     

    // Check if input and output files are provided
    if (inputFile.empty() || outputFile.empty()) {
        cerr << "Error: Input and output file options are required." << endl;
        return 1;
    }
    DEBUG_MSG("Algorthm Selected is " << algorithm);
    DEBUG_MSG("Inputfile Selected is " << inputFile);
    DEBUG_MSG("Outputfile Selected is " << outputFile);
    DEBUG_MSG("Numthreads Selected is " << NUM_THREADS);
    
    // Set the number of threads for OpenMP
    omp_set_num_threads(NUM_THREADS);

    // Sort and print the input file to the output file
    sortAndPrintFile(inputFile, outputFile, algorithm);

    return 0;
}

