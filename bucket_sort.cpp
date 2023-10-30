/*****************************************************************
 * @author Suraj Ajjampur
 * @file bucket_sort.cpp
 * 
 * @brief This C++ source file implements bucket sort using custom-built concurrency primitives
 * 
 * @date 24 Oct 2023
********************************************************************/

#include "bucket_sort.h"
#include "my_atomics.h"

using namespace std;

/**************** Globals *********************/
extern string lock_type;
extern string bar_type;
extern int NUM_THREADS;

pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER; // Initialize the pthread mutex
atomic<bool> lock_flag(false);
typedef struct ticket{
    atomic<int> next_num;
    atomic<int> now_serving;
}ticket_t;
ticket_t myTicket = {0, 0};
atomic<MCSLock::Node*> tail = nullptr;
MCSLock mcslock(tail);
pthread_barrier_t myBarrier;

/**
 * @brief Finds the maximum element in an integer vector.
 *
 * This function iterates through the given integer vector to find the maximum
 * element and returns its value. It considers the first 'n' elements in the
 * vector for the search.
 *
 * @param A The input integer vector.
 * @param n The number of elements in the vector to consider.
 * @return The maximum element in the vector 'A'.
 */
int Max(vector<int> A, int n) {
    int max = -32768; // Initialize 'max' to a small value

    // Iterate through the vector to find the maximum element
    for (int i = 0; i < n; i++) {
        if (A[i] > max) {
            max = A[i]; // Update 'max' if a larger element is found
        }
    }

    return max; // Return the maximum element found
}

// Linked List node
class Node {
public:
    int value;
    Node* next;
};

/**
 * @brief Inserts an element into a specific bucket in a multi-threaded environment.
 *
 * This function inserts an element into a specified bucket of the bins array.
 * It performs the insertion while respecting the chosen locking mechanism for
 * synchronization. Depending on the locking mechanism, threads may synchronize
 * before starting their work using different barriers.
 *
 * @param ptrBuckets  Pointer to the array of buckets.
 * @param idx         The index of the bucket where the element should be inserted.
 * @param tid         The thread ID of the calling thread.
 */
void Insert(Node** ptrBuckets, int idx, int tid) {
    Node* temp = new Node;
    temp->value = idx;
    temp->next = nullptr;

    // Create a Node for this thread (MCSLock specific)
    MCSLock::Node myNode;

    // Create a Petersons instance with specified consistency (if used)
    Petersons petersons(SEQ_CONISTENCY);

    // Create a static SenseBarrier (if used)
    static SenseBarrier barrier(NUM_THREADS);

    // Optionally, create a Petersons instance with RELEASE_CONSISTENCY
    if (lock_type == "petersonrel") {
        Petersons petersons(RELEASE_CONSISTENCY);
    }

    // Threads will synchronize here before starting their work
    if (bar_type == "pthread") {
        pthread_barrier_wait(&myBarrier);
    } else if (bar_type == "sense") {
        barrier.ArriveAndWait();
    }

    // Lock acquisition using different locking mechanisms
    if (lock_type == "tas") {
        tas_lock(lock_flag);
    } else if (lock_type == "pthread") {
        pthread_mutex_lock(&counter_lock); // Lock acquisition
    } else if (lock_type == "ttas") {
        ttas_lock(lock_flag);
    } else if (lock_type == "ticket") {
        ticket_lock(myTicket.next_num, myTicket.now_serving); // Lock acquisition
    } else if (lock_type == "mcs") {
        mcslock.acquire(&myNode);
    } else if (lock_type == "petersonseq" || lock_type == "petersonrel") {
        petersons.lock(tid);
    }

    // Actual insertion into the specified bucket
    if (ptrBuckets[idx] == nullptr) {
        ptrBuckets[idx] = temp;
    } else {
        Node* p = ptrBuckets[idx];
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = temp;
    }

    // Unlocking and releasing the lock, based on the chosen mechanism
    if (lock_type == "tas") {
        tas_unlock(lock_flag);
    } else if (lock_type == "pthread") {
        pthread_mutex_unlock(&counter_lock); // Unlock
    } else if (lock_type == "ttas") {
        ttas_unlock(lock_flag);
    } else if (lock_type == "ticket") {
        ticket_unlock(myTicket.now_serving); // Lock acquisition
    } else if (lock_type == "mcs") {
        mcslock.release(&myNode);
    } else if (lock_type == "petersonseq") {
        petersons.unlock(tid);
    } else if (lock_type == "petersonrel") {
        petersons.unlock(tid);
    }
}



void BucketSort(vector<int>& A, int NumThreads) {
    // Determine the maximum value in the input vector
    int max = Max(A, A.size());

    // Create bins array with one bucket per possible value
    vector<Node*> bins(max + 1, nullptr);

    // Create threads and launch the Insert function for each thread
    vector<thread> threads(NumThreads);
    int chunkSize = A.size() / NumThreads;

    // Initialize a barrier if using pthreads
    if (bar_type == "pthread") {
        pthread_barrier_init(&myBarrier, NULL, NUM_THREADS);
        DEBUG_MSG("pThread Barrier Enabled!!");
    }

    // Distribute elements into buckets in parallel
    for (int i = 0; i < NumThreads; i++) {
        int start = i * chunkSize;
        int end = (i == NumThreads - 1) ? A.size() : (i + 1) * chunkSize;

        // Create a thread that inserts elements into buckets
        threads[i] = thread([start, end, &bins, &A, i](int tid) {
            for (int j = start; j < end; j++) {
                int binIdx = A[j];
                Insert(&bins[0], binIdx, tid);
            }
        }, i);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NumThreads; i++) {
        threads[i].join();
    }

    // Destroy the barrier if using pthreads
    if (bar_type == "pthread") {
        pthread_barrier_destroy(&myBarrier);
    }

    // Update the input vector with the sorted elements from buckets
    int i = 0;
    int j = 0;
    while (i < max + 1) {
        while (bins[i] != nullptr) {
            A[j++] = bins[i]->value;
            Node* temp = bins[i];
            bins[i] = bins[i]->next;
            delete temp;
        }
        i++;
    }
}


