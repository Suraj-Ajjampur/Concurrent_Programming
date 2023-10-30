#include "bucket_sort.h"

using namespace std;

mutex mtx;

int Max(vector<int> A, int n) {
    int max = -32768;
    for (int i = 0; i < n; i++) {
        if (A[i] > max) {
            max = A[i];
        }
    }
    return max;
}

// Linked List node
class Node {
public:
    int value;
    Node* next;
};

void Insert(Node** ptrBuckets, int idx, int tid) {
    Node* temp = new Node;
    temp->value = idx;
    temp->next = nullptr;

    // Lock the mutex to protect the critical section
    unique_lock<mutex> lock(mtx);
    if (ptrBuckets[idx] == nullptr) {
        ptrBuckets[idx] = temp;
    } else {
        Node* p = ptrBuckets[idx];
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = temp;
    }
    // Unlock the mutex when done with the critical section
    lock.unlock();
}

void BucketSort(vector<int>& A, int NumThreads) {
    int max = Max(A, A.size());

    // Create bins array
    vector<Node*> bins(max + 1, nullptr);

    // Create threads and launch the Insert function
    vector<thread> threads(NumThreads);
    int chunkSize = A.size() / NumThreads;

    for (int i = 0; i < NumThreads; i++) {
        int start = i * chunkSize;
        int end = (i == NumThreads - 1) ? A.size() : (i + 1) * chunkSize;

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

    // Update A with sorted elements
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

