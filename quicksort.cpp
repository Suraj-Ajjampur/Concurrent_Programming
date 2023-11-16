/*****************************************************************
 * @author Suraj Ajjampur
 * @file quicksort.cpp
 * 
 * @brief OpenMP Multithreaded Quick Sort Algorrithm implementation in the recursive way 
 * 
 * @date 16 Nov 2023
 * 
 * @ref https://www.geeksforgeeks.org/multithreading-in-cpp/
********************************************************************/
#include "quicksort.h"

// Function to partition the vector and return the pivot index
int partition(std::vector<int>& arr, int left, int right) {
    int pivot = arr[right]; // Choose the last element as the pivot
    int i = left - 1; // // Initialize an index for elements less than or equal to the pivot

    for (int j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            i++; // Increment the index for elements less than or equal to the pivot
            std::swap(arr[i], arr[j]); // Swap arr[i] and arr[j] to move smaller elements to the left
        }
    }
    // Swap the pivot element with the element at the (i + 1)-th position
    std::swap(arr[i + 1], arr[right]);
    // Return the index of the pivot element
    return i + 1;
}

// Quicksort function
void quicksort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        // Partition the array and get the pivot index
        int pivotIndex = partition(arr, left, right);

        quicksort(arr, left, pivotIndex - 1);
        quicksort(arr, pivotIndex + 1, right);
    }
}
