#include "bucket_sort.h"
#include "my_atomics.h"

using namespace std;

//mutex mtx;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER; // Initialize the pthread mutex
extern string lock_type;
atomic<bool> lock_flag(false);
typedef struct ticket{
    atomic<int> next_num;
    atomic<int> now_serving;
}ticket_t;

ticket_t myTicket = {0, 0};
atomic<MCSLock::Node*> tail = nullptr;
MCSLock mcslock(tail);

pthread_barrier_t myBarrier;

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
    MCSLock::Node myNode; // Create a Node for this thread
    Petersons petersons(SEQ_CONISTENCY);
    if(lock_type == "petersonrel"){
        Petersons petersons(RELEASE_CONSISTENCY);
    }
    
    // Lock the mutex to protect the critical section
    if (lock_type == "tas") {
        tas_lock(lock_flag);
    }else if(lock_type == "pthread"){
        pthread_mutex_lock(&counter_lock); // Lock acquisition
    } else if(lock_type == "ttas"){
        ttas_lock(lock_flag);
    } else if(lock_type == "ticket"){
        ticket_lock(myTicket.next_num, myTicket.now_serving); // Lock acquisition
    } else if (lock_type == "mcs"){
        mcslock.acquire(&myNode);
    }else if (lock_type == "petersonrel" || lock_type == "petersonseq"){
        petersons.lock(tid);
    }
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
    if (lock_type == "tas") {
        tas_unlock(lock_flag);
    }else if(lock_type == "pthread"){
        pthread_mutex_unlock(&counter_lock); // Unlock
    }else if(lock_type == "ttas"){
        ttas_unlock(lock_flag);
    } else if(lock_type == "ticket"){
        ticket_unlock(myTicket.now_serving); // Lock acquisition
    } else if (lock_type == "mcs"){
        mcslock.release(&myNode);
    } else if (lock_type == "petersonseq"){
        petersons.unlock(tid);
    }else if (lock_type == "petersonrel"){
        petersons.unlock(tid);
    }
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

