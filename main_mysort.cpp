/*****************************************************************
 * @author Suraj Ajjampur
 * @file main_mysort.cpp
 * 
 * @brief Main file of the mysort program
 * 
 * @date 25 Oct 2023
********************************************************************/
// Includes
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include <ctime>
#include <fstream>
#include <pthread.h>
#include "my_atomics.h"
#include <getopt.h>
#include <cstring>
#include "bucket_sort.h"

// Global variables
int NUM_THREADS = 5;
string lock_type; 
string bar_type;

// Function to print my name
void printName() {
    cout << "Suraj Ajjampur" << endl;
}

/**
 * Function to sort integers from an input file using BucketSort and print the sorted integers to an output file.
 * 
 * @param inputFile The path to the input file containing unsorted integers.
 * @param outputFile The path to the output file where sorted integers will be written.
 */
void bucketSortAndPrint(const string& inputFile, const string& outputFile) {
    // Open the input file
    ifstream inFile(inputFile);
    DEBUG_MSG("Performing BucketSort\n");

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
    
    // Start measuring time
    auto start_time = chrono::high_resolution_clock::now();
    
    // Calling BucketSort function here and get the sorted result
    BucketSort(numbers, NUM_THREADS);
    
    if (numbers.empty()){
        return;
    }
    // Stop measuring time
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

    // Calculate and print elapsed time in nanoseconds
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

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

    string outputFile = "";
    string inputFile = "";

    // Parse command line arguments
    int c;

    // Define long options
    static struct option long_options[] = {
        {"name", no_argument, 0, 'm'},
        {"threads", required_argument, 0, 't'},
        {"num_iterations", required_argument, 0, 'n'},
        {"bar", required_argument, 0, 'b'},
        {"lock", required_argument, 0, 'l'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    while (1) {
        int option_index = 0;

        // Parse the command line options
        c = getopt_long(argc, argv, "mt:n:b:l:i:o:", long_options, &option_index);

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
            
            case 'b':
                //Set the barrier type
                bar_type = optarg;
                break;

            case 'l':
                // Set the lock type
                lock_type = optarg;
                break;

            case 'i':
                // Set the input file
                inputFile = optarg;
                cout << "The input file is" << inputFile << endl;

            case 'o':
                // Set the output file
                outputFile = optarg;
                cout << "The output file is " << outputFile << endl;
                break;

            case '?':
                // Handle invalid options
                cerr << "Error: Invalid option." << endl;
                return 1;

            default:
                abort();
        }
    }
    if (NUM_THREADS > 2 && (lock_type == "peterson" || lock_type == "petersonrel")){
        //Recalculating number of iterations accordingly
        DEBUG_MSG("The lock type is " << lock_type);
        NUM_THREADS = 2;
    }
    // Check if input and output files are provided
    if (inputFile.empty() || outputFile.empty()) {
        cerr << "Error: Output file options are required." << endl;
        return 1;
    }

    // Start measuring time
    auto start_time = chrono::high_resolution_clock::now();

    bucketSortAndPrint(inputFile, outputFile);

    return 0;
}