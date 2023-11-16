/* Include Guards */
#ifndef MERGESORT_H
#define MERGESORT_H

// Necessary headere file to use the 'std::vector' class.
#include <vector>

//Statement allows us to use the vector element with the 'std::' prefix within the functions that include this header.
using namespace std;

/* Function Prototypes*/
void merge(vector<int>& arr, int left, int middle, int right);
void mergesort(vector<int>& arr, int left, int right);

#endif // MERGESORT_H
