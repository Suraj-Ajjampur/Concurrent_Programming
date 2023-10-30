#include <iostream>
#include <vector>

using namespace std;

template <class T>
void Print(T& vec, string s) {
    cout << s << ": [";
    for (size_t i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

int Max(vector<int> A) {
    int max = -32768;
    for (int i = 0; i < A.size(); i++) {
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

void Insert(Node** ptrBins, int idx) {
    Node* temp = new Node;
    temp->value = idx;
    temp->next = nullptr;

    if (ptrBins[idx] == nullptr) { // ptrBins[idx] is head ptr
        ptrBins[idx] = temp;
    } else {
        Node* p = ptrBins[idx];
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = temp;
    }
}

int Delete(Node** ptrBins, int idx) {
    Node* p = ptrBins[idx];  // ptrBins[idx] is head ptr
    ptrBins[idx] = ptrBins[idx]->next;
    int x = p->value;
    delete p;
    return x;
}

void BucketSort(vector<int>& A, int NumThreads) {
    int max = Max(A);

    // Create bins array
    vector<Node*> bins(max + 1, nullptr);

    // Update count array values based on A values
    for (int i = 0; i < A.size(); i++) {
        Insert(&bins[0], A[i]);
    }

    // Update A with sorted elements
    int i = 0;
    int j = 0;
    while (i < max + 1) {
        while (bins[i] != nullptr) {
            A[j++] = Delete(&bins[0], i);
        }
        i++;
    }
}
