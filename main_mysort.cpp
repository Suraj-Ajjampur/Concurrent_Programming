#include "mysort.h"

// Function to print my name
void printName() {
    cout << "Suraj Ajjampur" << endl;
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
    int numThreads = 1; // Default number of threads is 5
    string algorithm = "forkjoin"; // Default algorithm is fork/join
    printf("Number of args is %d\n", argc);

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        printf("Processing for %s\n",argv[i]);

        if (arg == "--name") {
            // Print name
            printName();
            return 0;
        } else if (arg == "-i" && i + 1 < argc) {
            // Input file option
            inputFile = argv[++i];
            // printf("Processing for %s\n",inputFile);
            std::cout << "The i/p file is " << inputFile << std::endl;

        } else if (arg == "-o" && i + 1 < argc) {
            // Output file option
            outputFile = argv[++i];
            // printf("Processing for %s\n",outputFile);
            std::cout << "The o/p file is " << outputFile << std::endl;


        } else if (arg == "-t" && i + 1 < argc) {
            // Number of threads option
            int numThreads2 = atoi(argv[++i]);
            printf("Number of threads %d\n",numThreads2);
            numThreads = numThreads2;
            printf("numThreads Assigned to %d\n", numThreads);
        } else if (arg == "--alg" && i + 1 < argc) {
            // Algorithm option
            algorithm = argv[++i];
            cout << algorithm << endl;
        }
    }

    // Check if input and output files are provided
    if (inputFile.empty() || outputFile.empty()) {
        cerr << "Error: Input and output file options are required." << endl;
        return 1;
    }

    // Sort and print the input file using the selected algorithm
    if (algorithm == "forkjoin") {
        forkJoinSortAndPrint(inputFile, outputFile, numThreads);
    } else if (algorithm == "lkbucket") {
        bucketSortAndPrint(inputFile, outputFile, numThreads);
    } else {
        cerr << "Error: Invalid algorithm specified." << endl;
        return 1;
    }

    return 0;
}