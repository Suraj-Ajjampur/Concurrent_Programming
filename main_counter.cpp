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

using namespace std;

// Global variables
int cntr = 0;
atomic<bool> lock_flag(false);
int NUM_THREADS = 5;
int NUM_ITERATIONS = 10000;
string lock_type = "pthread"; 
string bar_type = "";
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER; // Initialize the pthread mutex
pthread_barrier_t myBarrier;
atomic<MCSLock::Node*> tail = nullptr;
MCSLock mcslock(tail);

typedef struct ticket{
    atomic<int> next_num;
    atomic<int> now_serving;
}ticket_t;

ticket_t myTicket = {0, 0};

// Function to increment the counter
void counter(int tid) {
    DEBUG_MSG("Thread ");
    static SenseBarrier barrier(NUM_THREADS);
    DEBUG_MSG(lock_type);
    // Threads will synchronize here before starting their work
    if(bar_type == "pthread"){
        pthread_barrier_wait(&myBarrier);
    }
    else if (bar_type == "sense"){
        barrier.ArriveAndWait();
    }

    if (lock_type == "tas") {
        //cout << "Test and Set\n" << endl;
        for (int i = 0; i < NUM_ITERATIONS; i++){
            tas_lock(lock_flag);
            // Critical Section - Increment the counter
            cntr++;
            tas_unlock(lock_flag);
        }
    }
    else if(lock_type == "ttas"){
        //cout << "Test and and Test and Set\n" << endl;
        for (int i = 0; i < NUM_ITERATIONS; i++){
            ttas_lock(lock_flag);
            // Critical Section - Increment the counter
            cntr++;
            ttas_unlock(lock_flag);
        }
    }
    else if (lock_type == "pthread") {
        //cout << "pThread\n" << endl;
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            pthread_mutex_lock(&counter_lock); // Lock acquisition

            // Critical Section - Increment the counter
            cntr++;

            pthread_mutex_unlock(&counter_lock); // Unlock
        }
    }
    else if (lock_type == "ticket") {
        //cout << "Ticket\n" << endl;

        for (int i = 0; i < NUM_ITERATIONS; i++) {
            ticket_lock(myTicket.next_num, myTicket.now_serving); // Lock acquisition

            // Critical Section - Increment the counter
            cntr++;

            ticket_unlock(myTicket.now_serving); // Unlock
        }
    }
    else if (lock_type == "mcs") {
        cout << "MCS Lock\n" << endl;
        MCSLock::Node myNode; // Create a Node for this thread

        for (int i = 0; i < NUM_ITERATIONS; i++) {
            // Acquire the lock to protect the critical section
            mcslock.acquire(&myNode);

            // Critical Section - Increment the counter
            cntr++;

            mcslock.release(&myNode);
        }
    }
    else if (lock_type == "peterson"){
        //cout << "Peterson's Sequential lock" << endl;
        Petersons petersons(SEQ_CONISTENCY);
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            // Acquire the lock to protect the critical section
            petersons.lock(tid);

            // Critical Section - Increment the counter
            cntr++;
            //cout << "counter val: " << cntr << endl;

            //Unlock here
            petersons.unlock(tid);
        }
    }
    else if (lock_type == "petersonrel"){
        //cout << "Peterson's Release Consistency lock" << endl;
        Petersons petersons(RELEASE_CONSISTENCY);
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            // Acquire the lock to protect the critical section
            petersons.lock(tid);

            // Critical Section - Increment the counter
            cntr++;
            //cout << "counter val: " << cntr << endl;

            //Unlock here
            petersons.unlock(tid);
        }
    }
    else {
        DEBUG_MSG("Invalid lock type: " << lock_type);
        return;
    }
    // Threads will synchronize here before exiting
    if(bar_type == "pthread"){
        pthread_barrier_wait(&myBarrier);
    }
    else if (bar_type == "sense"){
        barrier.ArriveAndWait();
    }
}

// Function to print my name
void printName() {
    cout << "Suraj Ajjampur" << endl;
}

int main(int argc, char* argv[]) {

    string outputFile = "";
    int numThreads = 1; // Default number of threads is 5

    // Parse command line arguments
    int c;

    // Define long options
    static struct option long_options[] = {
        {"name", no_argument, 0, 'm'},
        {"threads", required_argument, 0, 't'},
        {"num_iterations", required_argument, 0, 'n'},
        {"bar", required_argument, 0, 'b'},
        {"lock", required_argument, 0, 'l'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    while (1) {
        int option_index = 0;

        // Parse the command line options
        c = getopt_long(argc, argv, "mt:n:b:l:o:", long_options, &option_index);

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

            case 'n':
                // Set the number of iterations
                NUM_ITERATIONS = stoi(optarg);
                break;
            
            case 'b':
                //Set the barrier type
                bar_type = optarg;
                break;

            case 'l':
                // Set the lock type
                lock_type = optarg;
                break;

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
    // Check if input and output files are provided
    if (outputFile.empty()) {
        cerr << "Error: Output file options are required." << endl;
        return 1;
    }

    // Start measuring time
    auto start_time = chrono::high_resolution_clock::now();

    //Barrier Initialization
    if(bar_type == "pthread"){
    pthread_barrier_init(&myBarrier, NULL, NUM_THREADS);
    DEBUG_MSG("pThread Barrier Enabled!!");
    }

    // If user wants a lock constructed using peterson's algorithm
    // We have to reset number of threads to 2
    if (NUM_THREADS > 2 && (lock_type == "peterson" || lock_type == "petersonrel")){
        //Recalculating number of iterations accordingly
        NUM_ITERATIONS == NUM_ITERATIONS * (NUM_THREADS - 2);
        DEBUG_MSG("The lock type is " << lock_type);
        NUM_THREADS = 2;
    }

    // Create threads and launch the counter function
    thread threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = thread(counter, i);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }

    if(bar_type == "pthread"){
    pthread_barrier_destroy(&myBarrier);
    }

    // Stop measuring time
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);

    // Calculate and print elapsed time in nanoseconds
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    // Print the final counter value
    cout << "Final counter value: " << cntr << endl;

    // Open the output file for writing
    ofstream outputFileStream(outputFile);
    if (!outputFileStream) {
        cerr << "Error: Could not open output file for writing." << endl;
        return 1;
    }

    // Write the final counter value to the output file
    outputFileStream << cntr << endl;

    // Close the output file
    outputFileStream.close();

    return 0;
}